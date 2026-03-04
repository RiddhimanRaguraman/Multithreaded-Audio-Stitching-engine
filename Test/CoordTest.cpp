//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "UnitTestThread.h"
#include "TestData.h"
#include "File.h"

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

using namespace Azul;

//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST_WITH_TEARDOWN(Coord_Test, TestConfig::ALL)
{
#if Coord_Test

	// Verify the File is using Slow Reads
	unsigned int val;
	File::Magic(val);
	//Trace::out("magic: %x\n", val);
	CHECK(val == 0xA4B1);

	TestData *r = UnitTestThread::GetTestData();

	CHECK(r->pFileBuff != r->pCoordBuff_A);
	CHECK(r->pFileBuff != r->pCoordBuff_B);
	CHECK(r->pCoordBuff_A != r->pCoordBuff_B);

	CHECK(r->FileBuffSize == 512 * 1024);
	CHECK(r->CoordBuffSize_A == 512 * 1024);
	CHECK(r->CoordBuffSize_B == 512 * 1024);

	Trace::out("FileBuffer: %p  s:0x%X\n", r->pFileBuff, r->FileBuffSize);
	Trace::out("    CoordA: %p  s:0x%X\n", r->pCoordBuff_A, r->CoordBuffSize_A);
	Trace::out("    CoordB: %p  s:0x%X\n", r->pCoordBuff_B, r->CoordBuffSize_B);
	Trace::out("\n");

	for(size_t i = 0; i < r->Data.size(); i++)
	{
		TestData::Entry e = r->Data[i];
		int t_ms = AnimTime::Quotient(e.time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

		Trace::out("%02d) 0x%p s:0x%X(%d) crc:0x%08X %d %d ms\n", i,
				   e.pDataBuff, e.sizeData, e.sizeData, e.crc, e.name, t_ms);
	}
	Trace::out("\n");

	size_t i = 0;
	int tCurr = 0;
	int tOld = 0;

	int tMax = 0;
	int tMin = 0;
	int delta = 500;

	CHECK(i == 0);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x80000);
	CHECK(r->Data[i].crc == 0x295EAAAE);

	i++;

	CHECK(i == 1);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_B);
	CHECK(r->Data[i].name == TestData::BuffName::B);
	CHECK(r->Data[i].sizeData == 0x78000);
	CHECK(r->Data[i].crc == 0x8A23BDD4);

	tOld = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	i++;

	CHECK(i == 2);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x60000);
	CHECK(r->Data[i].crc == 0x0E77ACF7);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 3);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_B);
	CHECK(r->Data[i].name == TestData::BuffName::B);
	CHECK(r->Data[i].sizeData == 0x40000);
	CHECK(r->Data[i].crc == 0x8DD21B19);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;


	i++;

	CHECK(i == 4);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x7C000);
	CHECK(r->Data[i].crc == 0xD01E0081);


	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 5);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_B);
	CHECK(r->Data[i].name == TestData::BuffName::B);
	CHECK(r->Data[i].sizeData == 0x20000);
	CHECK(r->Data[i].crc == 0xAEE37AA9);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 6);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x60000);
	CHECK(r->Data[i].crc == 0x7DF652C1);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 7);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_B);
	CHECK(r->Data[i].name == TestData::BuffName::B);
	CHECK(r->Data[i].sizeData == 0x80000);
	CHECK(r->Data[i].crc == 0xADEE6397);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 8);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x70000);
	CHECK(r->Data[i].crc == 0x170654EF);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 9);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_B);
	CHECK(r->Data[i].name == TestData::BuffName::B);
	CHECK(r->Data[i].sizeData == 0x40000);
	CHECK(r->Data[i].crc == 0xDF7665C0);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 10);

	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x48000);
	CHECK(r->Data[i].crc == 0x300CD9A1);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 11);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_B);
	CHECK(r->Data[i].name == TestData::BuffName::B);
	CHECK(r->Data[i].sizeData == 0x20000);
	CHECK(r->Data[i].crc == 0x035CDE29);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 12);

	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x60000);
	CHECK(r->Data[i].crc == 0x3D8F1E9B);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 13);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_B);
	CHECK(r->Data[i].name == TestData::BuffName::B);
	CHECK(r->Data[i].sizeData == 0x20000);
	CHECK(r->Data[i].crc == 0x6E6611A1);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 14);

	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x70000);
	CHECK(r->Data[i].crc == 0xAC50EE8E);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 15);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_B);
	CHECK(r->Data[i].name == TestData::BuffName::B);
	CHECK(r->Data[i].sizeData == 0x7C000);
	CHECK(r->Data[i].crc == 0xD869806B);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 16);

	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x24000);
	CHECK(r->Data[i].crc == 0x162BE099);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 17);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_B);
	CHECK(r->Data[i].name == TestData::BuffName::B);
	CHECK(r->Data[i].sizeData == 0x60000);
	CHECK(r->Data[i].crc == 0x405CB153);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 18);

	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x78000);
	CHECK(r->Data[i].crc == 0x10793056);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 19);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_B);
	CHECK(r->Data[i].name == TestData::BuffName::B);
	CHECK(r->Data[i].sizeData == 0x70000);
	CHECK(r->Data[i].crc == 0xA1FEC2DC);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 20);

	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x70000);
	CHECK(r->Data[i].crc == 0xB0567482);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 21);
	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_B);
	CHECK(r->Data[i].name == TestData::BuffName::B);
	CHECK(r->Data[i].sizeData == 0x7C000);
	CHECK(r->Data[i].crc == 0xC6B67441);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	tOld = tCurr;

	i++;

	CHECK(i == 22);

	CHECK(r->Data[i].pDataBuff == r->pCoordBuff_A);
	CHECK(r->Data[i].name == TestData::BuffName::A);
	CHECK(r->Data[i].sizeData == 0x48E00);
	CHECK(r->Data[i].crc == 0xEA869D7E);

	tCurr = AnimTime::Quotient(r->Data[i].time, AnimTime(AnimTime::Duration::ONE_MILLISECOND));

	tMin = tOld + (int)(r->Data[i].sizeData) / 100;
	tMax = tMin + delta;

	Trace::out("%d) tMin:%d  < tCurr:%d < tMax:%d \n", i, tMin, tCurr, tMax);

	CHECK(tCurr > tMin);
	CHECK(tCurr < tMax);

	Trace::out("\n");


#endif
} TEST_END

TEST_TEARDOWN(Coord_Test)
{
#if Coord_Test



#endif
}

// --- End of File ---
