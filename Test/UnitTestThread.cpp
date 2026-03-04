//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "UnitTestThread.h"

// ----------------------------------
// Static initializations
// ----------------------------------
UnitTestThread *UnitTestThread::psUnitTestThread = nullptr;

// ----------------------------------
// Methods
// ----------------------------------

UnitTestThread::UnitTestThread(const char *const pName)
	: BannerBase(pName),
	mThread(),
	pFileBuff(nullptr),
	FileBuffSize(0),
	pCoordBuff_A(nullptr),
	CoordBuffSize_A(0),
	pCoordBuff_B(nullptr),
	CoordBuffSize_B(0),
	mWaveBuffReg{0},
	Data(),
	WaveData(),
	Timer(),
	buffCompletedCount(0),
	mWaveBuffCompletedCount(0),
	mtx(),
	cv()
{
	UnitTestThread::psUnitTestThread = this;
	Timer.Tic();
}

void UnitTestThread::operator()()
{
	START_BANNER;

	std::unique_lock<std::mutex> lck(this->mtx);
	this->cv.wait(lck,
				  [&]()
				  {
					  return true;// (this->buffCompletedCount == BUFFER_COUNT);
				  });

}


UnitTestThread::~UnitTestThread()
{
	if(this->mThread.joinable())
	{
		this->mThread.join();
		//Debug::out("~UnitTestThread()  join \n");
	}

	// here are where we launch the tests
	Test::RunTests();


}

void UnitTestThread::Launch()
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

UnitTestThread &UnitTestThread::privGetInstance()
{
	assert(UnitTestThread::psUnitTestThread);
	return *psUnitTestThread;
}

void UnitTestThread::File_BufferInfo(unsigned char *pBuff, size_t buffSize)
{
	UnitTestThread &r = UnitTestThread::privGetInstance();

	r.pFileBuff = pBuff;
	r.FileBuffSize = buffSize;
}

TestData *UnitTestThread::GetTestData()
{
	UnitTestThread &r = UnitTestThread::privGetInstance();
	TestData *p = (TestData *)&r;
	assert(p);
	return p;
}

UnitTestThread::WaveBuffReg *UnitTestThread::GetWaveBuffReg()
{
	UnitTestThread &r = UnitTestThread::privGetInstance();
	WaveBuffReg *p = &r.mWaveBuffReg[0];
	assert(p);
	return p;
}

void UnitTestThread::CoordA_BufferInfo(unsigned char *pBuff, size_t buffSize)
{
	UnitTestThread &r = UnitTestThread::privGetInstance();

	r.pCoordBuff_A = pBuff;
	r.CoordBuffSize_A = buffSize;
}

void UnitTestThread::CoordB_BufferInfo(unsigned char *pBuff, size_t buffSize)
{
	UnitTestThread &r = UnitTestThread::privGetInstance();

	r.pCoordBuff_B = pBuff;
	r.CoordBuffSize_B = buffSize;
}

void UnitTestThread::Wave_BufferInfo(unsigned char *_pBuff, size_t _buffSize, int _index)
{
	assert(_index >= 0);
	assert(_index < UnitTestThread::WAVE_COUNT);
	assert(_pBuff);

	UnitTestThread &r = UnitTestThread::privGetInstance();

	r.mWaveBuffReg[_index].pBuff = _pBuff;
	r.mWaveBuffReg[_index].BuffSize = _buffSize;
	r.mWaveBuffReg[_index].index = _index;
}

void UnitTestThread::DataTransfer(unsigned char *pBuffToTransfer, size_t TransferBuffSize)
{
	UnitTestThread &r = UnitTestThread::privGetInstance();

	Entry e;
	e.pDataBuff = pBuffToTransfer;
	e.sizeData = TransferBuffSize;
	e.time = r.Timer.Toc();
	e.crc = CRC32::GetCRC(pBuffToTransfer, TransferBuffSize);
	e.name = r.GetName(pBuffToTransfer);

	r.Data.push_back(e);

	r.buffCompletedCount++;

	if(r.buffCompletedCount >= UnitTestThread::BUFFER_COUNT)
	{
		r.cv.notify_one();
	}

}


void UnitTestThread::WaveDataTransfer(unsigned char *pDestBuff,
									  size_t DestBuff,
									  unsigned char *pSrcBuffToTransfer,
									  size_t SrcBuffToTransferSize)
{
	UnitTestThread &r = UnitTestThread::privGetInstance();

	WaveEntry e;
	e.pDesBuff = pDestBuff;
	e.DestDataSize = DestBuff;
	e.pSrcBuff = pSrcBuffToTransfer;
	e.SrcDataSize = SrcBuffToTransferSize;


	e.time = r.Timer.Toc();
	e.crc = CRC32::GetCRC(pSrcBuffToTransfer, SrcBuffToTransferSize);
	e.name = r.GetName(pDestBuff);


	//Trace::out("CHECK(r->WaveData[%d].crc == 0x%x);\n", r.mWaveBuffCompletedCount, e.crc);
	//Trace::out("CHECK(r->WaveData[%d].DestDataSize == 0x%x);\n", r.mWaveBuffCompletedCount, 2 * 1024);


	r.WaveData.push_back(e);

	r.mWaveBuffCompletedCount++;

	if(r.mWaveBuffCompletedCount >= UnitTestThread::WAVE_COUNT)
	{
		r.cv.notify_one();
	}

}
UnitTestThread::BuffName UnitTestThread::GetName(unsigned char *pBuff)
{
	BuffName name;

	if(pBuff == this->pCoordBuff_A)
	{
		name = UnitTestThread::BuffName::A;
	}
	else if(pBuff == this->pCoordBuff_B)
	{
		name = UnitTestThread::BuffName::B;
	}
	else if(pBuff == this->pFileBuff)
	{
		name = UnitTestThread::BuffName::File;
	}
	else
	{
		name = UnitTestThread::BuffName::Unknown;

		for(int i = 0; i < UnitTestThread::WAVE_COUNT; i++)
		{
			if(pBuff == mWaveBuffReg[i].pBuff)
			{
				// i is the index;
				name = (UnitTestThread::BuffName)((int)UnitTestThread::BuffName::Wave_0 + i);
				break;
			}
		}
	}

	return name;
}


// --- End of File ---
