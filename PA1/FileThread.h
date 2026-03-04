//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef FILE_THREAD_H
#define FILE_THREAD_H

#include "KillableBase.h"
#include "FileCoordShare.h"
#include "Buffer.h"

class FileThread : public KillableBase
{
public:
	static const unsigned int BUFFER_SIZE = 513 * 1024;
	static const unsigned int FILE_NAME_SIZE = 12;
	static const int MAX_NUM_WAVE_FILES = 23;

public:
	FileThread(const char *const pName,
			   FileCoordShare &share_fc);

	FileThread() = delete;
	FileThread(const FileThread &) = delete;
	FileThread &operator = (const FileThread &) = delete;
	~FileThread();

	void Launch();

	void operator()();

	static size_t CopyBufferToDest(Buffer *pDest);

	static int fileReadCount;
	static std::mutex fileCountMtx;
	static void IncrementFileReadCount();
	static int  GetFileReadCount();

private:
	char *privGetFileName(int index);
	void privLoadFile(const char *const pFileName);

	// needed for CopyBufferToDest
	static FileThread &privGetInstance();
	static FileThread *sInstance;
public:

	// --------------------------
	// Data
	// --------------------------

	std::thread mThread;

	char *poFileName;
	int numFilesRead;
	char pad[4];
	Buffer *poBuffer;

	FileCoordShare &share_fc;
};

#endif

// --- End of File ---
