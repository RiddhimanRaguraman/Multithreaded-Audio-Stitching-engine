//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "FileThread.h"
#include "File.h"
#include "UnitTestThread.h"
#include "KillThread.h"
#include "CoordThread.h"

using namespace Azul;

FileThread* FileThread::sInstance = nullptr;
int FileThread::fileReadCount = 0;
std::mutex FileThread::fileCountMtx;

void FileThread::IncrementFileReadCount()
{
    std::lock_guard<std::mutex> lk(FileThread::fileCountMtx);
    ++FileThread::fileReadCount;
}

int FileThread::GetFileReadCount()
{
    std::lock_guard<std::mutex> lk(FileThread::fileCountMtx);
    return FileThread::fileReadCount;
}

FileThread::FileThread(const char *const pName,
					   FileCoordShare &_share_fc)
	: KillableBase(pName),
	mThread(),
	poFileName(new char[FileThread::FILE_NAME_SIZE]()),
	numFilesRead(0),
	poBuffer(new Buffer(FileThread::BUFFER_SIZE, Buffer::Name::File)),
	share_fc(_share_fc)
{
	assert(poBuffer);
	assert(poFileName);

    FileThread::sInstance = this;

	UnitTestThread::File_BufferInfo(this->poBuffer->GetRawBuff(), this->poBuffer->GetTotalSize());

	std::lock_guard<std::mutex> lck(share_fc.mtx);
	share_fc.status = FileCoordShare::Status::Empty;
}


void FileThread::operator()()
{
	START_BANNER;

	while(!KillEvent())
	{
		{ 
			std::unique_lock<std::mutex> lck(share_fc.mtx);
			if(share_fc.status == FileCoordShare::Status::Empty)
			{
				lck.unlock();  // <-- do not hold during load otherwise stall Coord thread

				// Sleep only after Coordinator completed a transfer (status==Empty)
				// Pace between files to avoid simultaneous front+back starvation
				if(this->numFilesRead > 0)
				{
					std::this_thread::sleep_for(200ms);
				}
				if(this->numFilesRead < FileThread::MAX_NUM_WAVE_FILES)
				{
					int i = this->numFilesRead;
					char *pName = this->privGetFileName(i);
					this->privLoadFile(pName);

                    // Increment the static file read count atomically
					FileThread::IncrementFileReadCount();

					lck.lock();  // changing status... protect it
					share_fc.status = FileCoordShare::Status::Ready;
					//Debug::out("FileCoordShare::Status::Ready\n");
					lck.unlock();  // we learned to unlock before notify

					share_fc.cv.notify_one();
					// Wake Coordinator's event loop as well (state machine waits on evtCv)
					CoordThread::NotifyCoordWork();
				}
			}
		}


	}
}

void FileThread::privLoadFile(const char *const pFileName)
{
	File::Handle fh;
	File::Error err;

	assert(pFileName);

	Debug::out("%s load start <-- \n", pFileName);

	err = File::Open(fh, pFileName, File::Mode::READ);
	assert(err == File::Error::SUCCESS);

	err = File::Seek(fh, File::Position::END, 0);
	assert(err == File::Error::SUCCESS);

	DWORD size;
	err = File::Tell(fh, size);
	assert(err == File::Error::SUCCESS);

	err = File::Seek(fh, File::Position::BEGIN, 0);
	assert(err == File::Error::SUCCESS);

	err = File::Read(fh, poBuffer->GetRawBuff(), size);
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	poBuffer->SetCurrSize(size);
	this->numFilesRead++;

	Debug::out("%s load end --> %d bytes \n", pFileName, poBuffer->GetCurrSize());
}

char *FileThread::privGetFileName(int index)
{
	sprintf_s(poFileName, FileThread::FILE_NAME_SIZE, "wave_%d.wav", index);
	return poFileName;
}

FileThread::~FileThread()
{
	if(this->mThread.joinable())
	{
		this->mThread.join();
		Debug::out("~FileThread()  join \n");
	}

	delete poBuffer;
	poBuffer = nullptr;

	delete poFileName;
	poFileName = nullptr;
}

void FileThread::Launch()
{
	if(this->mThread.joinable() == false)
	{
		this->mThread = std::thread(std::ref(*this));
	}
	else
	{
		assert(false);
	}
}

FileThread &FileThread::privGetInstance()
{
    assert(FileThread::sInstance);
    return *FileThread::sInstance;
}

size_t FileThread::CopyBufferToDest(Buffer *pDest)
{
	assert(pDest);

	FileThread &r = FileThread::privGetInstance();

	std::lock_guard<std::mutex> lock(r.poBuffer->GetMutuex());

	memcpy_s(pDest->GetRawBuff(),
			 pDest->GetTotalSize(),
			 r.poBuffer->GetRawBuff(),
			 r.poBuffer->GetCurrSize());

	pDest->SetCurrSize(r.poBuffer->GetCurrSize());

	return pDest->GetCurrSize();
}

// --- End of File ---
