//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef UNIT_TEST_THREAD_H
#define UNIT_TEST_THREAD_H

#include "AnimTimer.h"
#include "TestData.h"

class UnitTestThread : public BannerBase
{
public:
	friend struct TestData;

	static const uint32_t BUFFER_COUNT = 23;
	static const uint32_t WAVE_COUNT = 20;
	static const uint32_t WAVE_BUFF_SIZE = 2 * 1024;

	enum BuffName
	{
		File,
		A,
		B,
		Wave_0,
		Wave_1,
		Wave_2,
		Wave_3,
		Wave_4,
		Wave_5,
		Wave_6,
		Wave_7,
		Wave_8,
		Wave_9,
		Wave_10,
		Wave_11,
		Wave_12,
		Wave_13,
		Wave_14,
		Wave_15,
		Wave_16,
		Wave_17,
		Wave_18,
		Wave_19,
		Unknown
	};

	struct Entry
	{
		unsigned char *pDataBuff;
		size_t          sizeData;
		Azul::AnimTime  time;
		uint32_t        crc;
		BuffName		name;
	};

	struct WaveEntry
	{
		unsigned char *pSrcBuff;
		size_t         SrcDataSize;
		unsigned char *pDesBuff;
		size_t         DestDataSize;
		Azul::AnimTime  time;
		uint32_t        crc;
		BuffName		name;
	};

	struct WaveBuffReg
	{
		unsigned char *pBuff;
		size_t BuffSize;
		int index;
		char pad[4];
	};

public:
	UnitTestThread() = delete;
	UnitTestThread(const UnitTestThread &) = delete;
	UnitTestThread &operator = (const UnitTestThread &) = delete;
	virtual ~UnitTestThread();

	UnitTestThread(const char *const pName);

	void Launch();
	void operator()();

	static TestData *GetTestData();
	static WaveBuffReg *GetWaveBuffReg();

	// USE these functions for Unit Test
	// Call them directly inside your code

	// PART A
	// All of these functions need to be called for testing
	static void File_BufferInfo(unsigned char *pBuff, size_t buffSize);
	static void CoordA_BufferInfo(unsigned char *pBuff, size_t buffSize);
	static void CoordB_BufferInfo(unsigned char *pBuff, size_t buffSize);
	static void DataTransfer(unsigned char *pBuffToTransfer, size_t TransferBuffSize);

	// PART B
	// register the wave buffer, index 0-19
	static void Wave_BufferInfo(unsigned char *pBuff, size_t buffSize, int index);
	static void WaveDataTransfer(unsigned char *pDestBuffToTransfer,
								 size_t DestBuffToTransferSize,
								 unsigned char *pSrcBuffToTransfer,
								 size_t SrcBuffToTransferSize);

private:
	// needed for Static Methods
	static UnitTestThread &privGetInstance();
	static UnitTestThread *psUnitTestThread;
	BuffName GetName(unsigned char *pBuff);

	// --------------------------
	// Data
	// --------------------------
private:

	std::thread mThread;


	unsigned char *pFileBuff;
	size_t FileBuffSize;

	unsigned char *pCoordBuff_A;
	size_t CoordBuffSize_A;

	unsigned char *pCoordBuff_B;
	size_t CoordBuffSize_B;

	WaveBuffReg mWaveBuffReg[WAVE_COUNT];

	std::vector<Entry> Data;
	std::vector<WaveEntry> WaveData;

	Azul::AnimTimer    Timer;

	int buffCompletedCount;
	int mWaveBuffCompletedCount;
	std::mutex				 mtx;
	std::condition_variable  cv;
};

#endif

// --- End of File ---
