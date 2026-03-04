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

TEST_WITH_TEARDOWN(Wave_Test, TestConfig::ALL)
{
#if Wave_Test

	// Verify the File is using Slow Reads
	unsigned int val;
	File::Magic(val);
	//Trace::out("magic: %x\n", val);
	CHECK(val == 0xA4B1);

	TestData *r = UnitTestThread::GetTestData();

	// make sure all the wave buffers are unique and are 2K
	for(int i = 0; i < UnitTestThread::WAVE_COUNT; i++)
	{
		//Debug::out("i(%d) (%p)\n", i, r->mWaveBuffReg[i].pBuff);
		CHECK(r->mWaveBuffReg[i].BuffSize == UnitTestThread::WAVE_BUFF_SIZE);
		CHECK(r->mWaveBuffReg[i].index == i);

		for(int j = i + 1; j < UnitTestThread::WAVE_COUNT; j++)
		{
			CHECK(r->mWaveBuffReg[i].pBuff != r->mWaveBuffReg[j].pBuff);
			//Debug::out("(%d,%d) : %d\n", i,j,r->mWaveBuffReg[i].index);
		}
	}

	//Debug::out("---\n");
	// Make sure multiple buffers are evenly used
	for(size_t j = 0; j < UnitTestThread::WAVE_COUNT; j++)
	{
		int count = 0;
		unsigned char *pRefDataBuff = r->mWaveBuffReg[j].pBuff;

		for(size_t i = 0; i < r->WaveData.size(); i++)
		{
			if(r->WaveData[i].pDestBuff == pRefDataBuff)
			{
				count++;
			}
		}
		CHECK(count >= 12);
		//Debug::out("%d: (%p): %d\n",j, pRefDataBuff, count);
	}


	CHECK(r->WaveData[0].crc == 0xf1e8ba9e);
	CHECK(r->WaveData[0].DestDataSize == 0x800);

	CHECK(r->WaveData[1].crc == 0xf1e8ba9e);
	CHECK(r->WaveData[1].DestDataSize == 0x800);

	CHECK(r->WaveData[2].crc == 0x3fe971f9);
	CHECK(r->WaveData[2].DestDataSize == 0x800);

	CHECK(r->WaveData[3].crc == 0x4bcc5551);
	CHECK(r->WaveData[3].DestDataSize == 0x800);

	CHECK(r->WaveData[4].crc == 0xe1c7abb4);
	CHECK(r->WaveData[4].DestDataSize == 0x800);

	CHECK(r->WaveData[5].crc == 0xfe3e5eb3);
	CHECK(r->WaveData[5].DestDataSize == 0x800);

	CHECK(r->WaveData[6].crc == 0xcc47859e);
	CHECK(r->WaveData[6].DestDataSize == 0x800);

	CHECK(r->WaveData[7].crc == 0x1917c502);
	CHECK(r->WaveData[7].DestDataSize == 0x800);

	CHECK(r->WaveData[8].crc == 0x45aa0b49);
	CHECK(r->WaveData[8].DestDataSize == 0x800);

	CHECK(r->WaveData[9].crc == 0x216d7307);
	CHECK(r->WaveData[9].DestDataSize == 0x800);

	CHECK(r->WaveData[10].crc == 0x51b74206);
	CHECK(r->WaveData[10].DestDataSize == 0x800);

	CHECK(r->WaveData[11].crc == 0x446aae9b);
	CHECK(r->WaveData[11].DestDataSize == 0x800);

	CHECK(r->WaveData[12].crc == 0xc3ca0468);
	CHECK(r->WaveData[12].DestDataSize == 0x800);

	CHECK(r->WaveData[13].crc == 0xf69d5791);
	CHECK(r->WaveData[13].DestDataSize == 0x800);

	CHECK(r->WaveData[14].crc == 0xb38a363f);
	CHECK(r->WaveData[14].DestDataSize == 0x800);

	CHECK(r->WaveData[15].crc == 0xf8800d62);
	CHECK(r->WaveData[15].DestDataSize == 0x800);

	CHECK(r->WaveData[16].crc == 0x92dac01d);
	CHECK(r->WaveData[16].DestDataSize == 0x800);

	CHECK(r->WaveData[17].crc == 0x88bc526c);
	CHECK(r->WaveData[17].DestDataSize == 0x800);

	CHECK(r->WaveData[18].crc == 0x1572451c);
	CHECK(r->WaveData[18].DestDataSize == 0x800);

	CHECK(r->WaveData[19].crc == 0x5b4ed03);
	CHECK(r->WaveData[19].DestDataSize == 0x800);

	CHECK(r->WaveData[20].crc == 0x8ae7e0e5);
	CHECK(r->WaveData[20].DestDataSize == 0x800);

	CHECK(r->WaveData[21].crc == 0x7058b65e);
	CHECK(r->WaveData[21].DestDataSize == 0x800);

	CHECK(r->WaveData[22].crc == 0x143577af);
	CHECK(r->WaveData[22].DestDataSize == 0x800);

	CHECK(r->WaveData[23].crc == 0x8df0f371);
	CHECK(r->WaveData[23].DestDataSize == 0x800);

	CHECK(r->WaveData[24].crc == 0xfe735bb9);
	CHECK(r->WaveData[24].DestDataSize == 0x800);

	CHECK(r->WaveData[25].crc == 0x62e584a9);
	CHECK(r->WaveData[25].DestDataSize == 0x800);

	CHECK(r->WaveData[26].crc == 0x2dd542ac);
	CHECK(r->WaveData[26].DestDataSize == 0x800);

	CHECK(r->WaveData[27].crc == 0xc9cc332);
	CHECK(r->WaveData[27].DestDataSize == 0x800);

	CHECK(r->WaveData[28].crc == 0xad198c3f);
	CHECK(r->WaveData[28].DestDataSize == 0x800);

	CHECK(r->WaveData[29].crc == 0xfe98192e);
	CHECK(r->WaveData[29].DestDataSize == 0x800);

	CHECK(r->WaveData[30].crc == 0xc6edb3d6);
	CHECK(r->WaveData[30].DestDataSize == 0x800);

	CHECK(r->WaveData[31].crc == 0x56b06230);
	CHECK(r->WaveData[31].DestDataSize == 0x800);

	CHECK(r->WaveData[32].crc == 0x4dbbf5f8);
	CHECK(r->WaveData[32].DestDataSize == 0x800);

	CHECK(r->WaveData[33].crc == 0xcf3871c3);
	CHECK(r->WaveData[33].DestDataSize == 0x800);

	CHECK(r->WaveData[34].crc == 0x34e44af4);
	CHECK(r->WaveData[34].DestDataSize == 0x800);

	CHECK(r->WaveData[35].crc == 0x628011cc);
	CHECK(r->WaveData[35].DestDataSize == 0x800);

	CHECK(r->WaveData[36].crc == 0xf3806747);
	CHECK(r->WaveData[36].DestDataSize == 0x800);

	CHECK(r->WaveData[37].crc == 0xff748ea);
	CHECK(r->WaveData[37].DestDataSize == 0x800);

	CHECK(r->WaveData[38].crc == 0x855995af);
	CHECK(r->WaveData[38].DestDataSize == 0x800);

	CHECK(r->WaveData[39].crc == 0x76a64902);
	CHECK(r->WaveData[39].DestDataSize == 0x800);

	CHECK(r->WaveData[40].crc == 0x7ff2b328);
	CHECK(r->WaveData[40].DestDataSize == 0x800);

	CHECK(r->WaveData[41].crc == 0xebd499b);
	CHECK(r->WaveData[41].DestDataSize == 0x800);

	CHECK(r->WaveData[42].crc == 0x88a93a53);
	CHECK(r->WaveData[42].DestDataSize == 0x800);

	CHECK(r->WaveData[43].crc == 0x5578529b);
	CHECK(r->WaveData[43].DestDataSize == 0x800);

	CHECK(r->WaveData[44].crc == 0xbe797e32);
	CHECK(r->WaveData[44].DestDataSize == 0x800);

	CHECK(r->WaveData[45].crc == 0x359f96a4);
	CHECK(r->WaveData[45].DestDataSize == 0x800);

	CHECK(r->WaveData[46].crc == 0xe430e351);
	CHECK(r->WaveData[46].DestDataSize == 0x800);

	CHECK(r->WaveData[47].crc == 0x917c1bbe);
	CHECK(r->WaveData[47].DestDataSize == 0x800);

	CHECK(r->WaveData[48].crc == 0x52bda9b8);
	CHECK(r->WaveData[48].DestDataSize == 0x800);

	CHECK(r->WaveData[49].crc == 0x8c289a58);
	CHECK(r->WaveData[49].DestDataSize == 0x800);

	CHECK(r->WaveData[50].crc == 0x361bd323);
	CHECK(r->WaveData[50].DestDataSize == 0x800);

	CHECK(r->WaveData[51].crc == 0xcc78d8f2);
	CHECK(r->WaveData[51].DestDataSize == 0x800);

	CHECK(r->WaveData[52].crc == 0x2e80902f);
	CHECK(r->WaveData[52].DestDataSize == 0x800);

	CHECK(r->WaveData[53].crc == 0x2b0a5eb6);
	CHECK(r->WaveData[53].DestDataSize == 0x800);

	CHECK(r->WaveData[54].crc == 0xbd44d3ca);
	CHECK(r->WaveData[54].DestDataSize == 0x800);

	CHECK(r->WaveData[55].crc == 0x993d50c1);
	CHECK(r->WaveData[55].DestDataSize == 0x800);

	CHECK(r->WaveData[56].crc == 0x2d2e71f1);
	CHECK(r->WaveData[56].DestDataSize == 0x800);

	CHECK(r->WaveData[57].crc == 0x4c610cbe);
	CHECK(r->WaveData[57].DestDataSize == 0x800);

	CHECK(r->WaveData[58].crc == 0x7e97c673);
	CHECK(r->WaveData[58].DestDataSize == 0x800);

	CHECK(r->WaveData[59].crc == 0xc59d9624);
	CHECK(r->WaveData[59].DestDataSize == 0x800);

	CHECK(r->WaveData[60].crc == 0xec3a26ab);
	CHECK(r->WaveData[60].DestDataSize == 0x800);

	CHECK(r->WaveData[61].crc == 0xa948c20a);
	CHECK(r->WaveData[61].DestDataSize == 0x800);

	CHECK(r->WaveData[62].crc == 0xf1fc64dc);
	CHECK(r->WaveData[62].DestDataSize == 0x800);

	CHECK(r->WaveData[63].crc == 0x3bc1a66e);
	CHECK(r->WaveData[63].DestDataSize == 0x800);

	CHECK(r->WaveData[64].crc == 0x711a7540);
	CHECK(r->WaveData[64].DestDataSize == 0x800);

	CHECK(r->WaveData[65].crc == 0x8d31d2f6);
	CHECK(r->WaveData[65].DestDataSize == 0x800);

	CHECK(r->WaveData[66].crc == 0xdac01ae6);
	CHECK(r->WaveData[66].DestDataSize == 0x800);

	CHECK(r->WaveData[67].crc == 0x3bb0c238);
	CHECK(r->WaveData[67].DestDataSize == 0x800);

	CHECK(r->WaveData[68].crc == 0x77453ef3);
	CHECK(r->WaveData[68].DestDataSize == 0x800);

	CHECK(r->WaveData[69].crc == 0xf1b41c87);
	CHECK(r->WaveData[69].DestDataSize == 0x800);

	CHECK(r->WaveData[70].crc == 0x88415d8c);
	CHECK(r->WaveData[70].DestDataSize == 0x800);

	CHECK(r->WaveData[71].crc == 0x69d5e647);
	CHECK(r->WaveData[71].DestDataSize == 0x800);

	CHECK(r->WaveData[72].crc == 0xcc7dfb83);
	CHECK(r->WaveData[72].DestDataSize == 0x800);

	CHECK(r->WaveData[73].crc == 0x418cedc9);
	CHECK(r->WaveData[73].DestDataSize == 0x800);

	CHECK(r->WaveData[74].crc == 0xdac03f39);
	CHECK(r->WaveData[74].DestDataSize == 0x800);

	CHECK(r->WaveData[75].crc == 0x2ae1e86a);
	CHECK(r->WaveData[75].DestDataSize == 0x800);

	CHECK(r->WaveData[76].crc == 0x2321f070);
	CHECK(r->WaveData[76].DestDataSize == 0x800);

	CHECK(r->WaveData[77].crc == 0x214f9dcf);
	CHECK(r->WaveData[77].DestDataSize == 0x800);

	CHECK(r->WaveData[78].crc == 0x6ecbbe6);
	CHECK(r->WaveData[78].DestDataSize == 0x800);

	CHECK(r->WaveData[79].crc == 0xef0f8b36);
	CHECK(r->WaveData[79].DestDataSize == 0x800);

	CHECK(r->WaveData[80].crc == 0x600b7640);
	CHECK(r->WaveData[80].DestDataSize == 0x800);

	CHECK(r->WaveData[81].crc == 0xfd17af9b);
	CHECK(r->WaveData[81].DestDataSize == 0x800);

	CHECK(r->WaveData[82].crc == 0x7e9d6513);
	CHECK(r->WaveData[82].DestDataSize == 0x800);

	CHECK(r->WaveData[83].crc == 0x9bc9eed7);
	CHECK(r->WaveData[83].DestDataSize == 0x800);

	CHECK(r->WaveData[84].crc == 0xf315969);
	CHECK(r->WaveData[84].DestDataSize == 0x800);

	CHECK(r->WaveData[85].crc == 0xbfab0843);
	CHECK(r->WaveData[85].DestDataSize == 0x800);

	CHECK(r->WaveData[86].crc == 0x4a75a850);
	CHECK(r->WaveData[86].DestDataSize == 0x800);

	CHECK(r->WaveData[87].crc == 0x711b0904);
	CHECK(r->WaveData[87].DestDataSize == 0x800);

	CHECK(r->WaveData[88].crc == 0xa34b3626);
	CHECK(r->WaveData[88].DestDataSize == 0x800);

	CHECK(r->WaveData[89].crc == 0x32f85784);
	CHECK(r->WaveData[89].DestDataSize == 0x800);

	CHECK(r->WaveData[90].crc == 0x277639e5);
	CHECK(r->WaveData[90].DestDataSize == 0x800);

	CHECK(r->WaveData[91].crc == 0xa85275a5);
	CHECK(r->WaveData[91].DestDataSize == 0x800);

	CHECK(r->WaveData[92].crc == 0x40476ca6);
	CHECK(r->WaveData[92].DestDataSize == 0x800);

	CHECK(r->WaveData[93].crc == 0x9fe5fdaa);
	CHECK(r->WaveData[93].DestDataSize == 0x800);

	CHECK(r->WaveData[94].crc == 0x7ae02a2f);
	CHECK(r->WaveData[94].DestDataSize == 0x800);

	CHECK(r->WaveData[95].crc == 0x8e75a5bb);
	CHECK(r->WaveData[95].DestDataSize == 0x800);

	CHECK(r->WaveData[96].crc == 0x95dc069b);
	CHECK(r->WaveData[96].DestDataSize == 0x800);

	CHECK(r->WaveData[97].crc == 0x460ef7b5);
	CHECK(r->WaveData[97].DestDataSize == 0x800);

	CHECK(r->WaveData[98].crc == 0xcf173793);
	CHECK(r->WaveData[98].DestDataSize == 0x800);

	CHECK(r->WaveData[99].crc == 0x24f4fcd4);
	CHECK(r->WaveData[99].DestDataSize == 0x800);

	CHECK(r->WaveData[100].crc == 0x1c404ec1);
	CHECK(r->WaveData[100].DestDataSize == 0x800);

	CHECK(r->WaveData[101].crc == 0x4937a008);
	CHECK(r->WaveData[101].DestDataSize == 0x800);

	CHECK(r->WaveData[102].crc == 0xc9bf61a8);
	CHECK(r->WaveData[102].DestDataSize == 0x800);

	CHECK(r->WaveData[103].crc == 0xd8775ead);
	CHECK(r->WaveData[103].DestDataSize == 0x800);

	CHECK(r->WaveData[104].crc == 0x38e49785);
	CHECK(r->WaveData[104].DestDataSize == 0x800);

	CHECK(r->WaveData[105].crc == 0x5f0c1b44);
	CHECK(r->WaveData[105].DestDataSize == 0x800);

	CHECK(r->WaveData[106].crc == 0xf8a6d272);
	CHECK(r->WaveData[106].DestDataSize == 0x800);

	CHECK(r->WaveData[107].crc == 0xc9aa04d3);
	CHECK(r->WaveData[107].DestDataSize == 0x800);

	CHECK(r->WaveData[108].crc == 0xe1c41c00);
	CHECK(r->WaveData[108].DestDataSize == 0x800);

	CHECK(r->WaveData[109].crc == 0x1906153d);
	CHECK(r->WaveData[109].DestDataSize == 0x800);

	CHECK(r->WaveData[110].crc == 0xbac08c62);
	CHECK(r->WaveData[110].DestDataSize == 0x800);

	CHECK(r->WaveData[111].crc == 0xddc021c9);
	CHECK(r->WaveData[111].DestDataSize == 0x800);

	CHECK(r->WaveData[112].crc == 0x911eac67);
	CHECK(r->WaveData[112].DestDataSize == 0x800);

	CHECK(r->WaveData[113].crc == 0x688ef405);
	CHECK(r->WaveData[113].DestDataSize == 0x800);

	CHECK(r->WaveData[114].crc == 0x53a304e2);
	CHECK(r->WaveData[114].DestDataSize == 0x800);

	CHECK(r->WaveData[115].crc == 0x5a56589);
	CHECK(r->WaveData[115].DestDataSize == 0x800);

	CHECK(r->WaveData[116].crc == 0xa54ffff1);
	CHECK(r->WaveData[116].DestDataSize == 0x800);

	CHECK(r->WaveData[117].crc == 0xc86210be);
	CHECK(r->WaveData[117].DestDataSize == 0x800);

	CHECK(r->WaveData[118].crc == 0x70080425);
	CHECK(r->WaveData[118].DestDataSize == 0x800);

	CHECK(r->WaveData[119].crc == 0x43b74e99);
	CHECK(r->WaveData[119].DestDataSize == 0x800);

	CHECK(r->WaveData[120].crc == 0x9b0f6ad8);
	CHECK(r->WaveData[120].DestDataSize == 0x800);

	CHECK(r->WaveData[121].crc == 0xc030b162);
	CHECK(r->WaveData[121].DestDataSize == 0x800);

	CHECK(r->WaveData[122].crc == 0xc56120a);
	CHECK(r->WaveData[122].DestDataSize == 0x800);

	CHECK(r->WaveData[123].crc == 0xa8cba649);
	CHECK(r->WaveData[123].DestDataSize == 0x800);

	CHECK(r->WaveData[124].crc == 0xaac3202);
	CHECK(r->WaveData[124].DestDataSize == 0x800);

	CHECK(r->WaveData[125].crc == 0x7829d18d);
	CHECK(r->WaveData[125].DestDataSize == 0x800);

	CHECK(r->WaveData[126].crc == 0x9dd9a42b);
	CHECK(r->WaveData[126].DestDataSize == 0x800);

	CHECK(r->WaveData[127].crc == 0xe82817e);
	CHECK(r->WaveData[127].DestDataSize == 0x800);

	CHECK(r->WaveData[128].crc == 0x80d28ce0);
	CHECK(r->WaveData[128].DestDataSize == 0x800);

	CHECK(r->WaveData[129].crc == 0xd9e06d3);
	CHECK(r->WaveData[129].DestDataSize == 0x800);

	CHECK(r->WaveData[130].crc == 0x6dee3661);
	CHECK(r->WaveData[130].DestDataSize == 0x800);

	CHECK(r->WaveData[131].crc == 0xfe9c798e);
	CHECK(r->WaveData[131].DestDataSize == 0x800);

	CHECK(r->WaveData[132].crc == 0x84273c10);
	CHECK(r->WaveData[132].DestDataSize == 0x800);

	CHECK(r->WaveData[133].crc == 0xa8d5ef07);
	CHECK(r->WaveData[133].DestDataSize == 0x800);

	CHECK(r->WaveData[134].crc == 0x735969b1);
	CHECK(r->WaveData[134].DestDataSize == 0x800);

	CHECK(r->WaveData[135].crc == 0x6d983dd7);
	CHECK(r->WaveData[135].DestDataSize == 0x800);

	CHECK(r->WaveData[136].crc == 0xcccb2427);
	CHECK(r->WaveData[136].DestDataSize == 0x800);

	CHECK(r->WaveData[137].crc == 0xb80ff428);
	CHECK(r->WaveData[137].DestDataSize == 0x800);

	CHECK(r->WaveData[138].crc == 0x368fdf1e);
	CHECK(r->WaveData[138].DestDataSize == 0x800);

	CHECK(r->WaveData[139].crc == 0xbcbfe7d4);
	CHECK(r->WaveData[139].DestDataSize == 0x800);

	CHECK(r->WaveData[140].crc == 0x7219f89a);
	CHECK(r->WaveData[140].DestDataSize == 0x800);

	CHECK(r->WaveData[141].crc == 0x9b12fe7a);
	CHECK(r->WaveData[141].DestDataSize == 0x800);

	CHECK(r->WaveData[142].crc == 0xda4fa9d9);
	CHECK(r->WaveData[142].DestDataSize == 0x800);

	CHECK(r->WaveData[143].crc == 0xef2f56);
	CHECK(r->WaveData[143].DestDataSize == 0x800);

	CHECK(r->WaveData[144].crc == 0xfa6bec8a);
	CHECK(r->WaveData[144].DestDataSize == 0x800);

	CHECK(r->WaveData[145].crc == 0xb68d6680);
	CHECK(r->WaveData[145].DestDataSize == 0x800);

	CHECK(r->WaveData[146].crc == 0xe027242d);
	CHECK(r->WaveData[146].DestDataSize == 0x800);

	CHECK(r->WaveData[147].crc == 0x2759e2ff);
	CHECK(r->WaveData[147].DestDataSize == 0x800);

	CHECK(r->WaveData[148].crc == 0xa32be34d);
	CHECK(r->WaveData[148].DestDataSize == 0x800);

	CHECK(r->WaveData[149].crc == 0x784684ba);
	CHECK(r->WaveData[149].DestDataSize == 0x800);

	CHECK(r->WaveData[150].crc == 0x275c2e87);
	CHECK(r->WaveData[150].DestDataSize == 0x800);

	CHECK(r->WaveData[151].crc == 0x4260f3df);
	CHECK(r->WaveData[151].DestDataSize == 0x800);

	CHECK(r->WaveData[152].crc == 0x247736dc);
	CHECK(r->WaveData[152].DestDataSize == 0x800);

	CHECK(r->WaveData[153].crc == 0x900ccb4b);
	CHECK(r->WaveData[153].DestDataSize == 0x800);

	CHECK(r->WaveData[154].crc == 0x166b4fd5);
	CHECK(r->WaveData[154].DestDataSize == 0x800);

	CHECK(r->WaveData[155].crc == 0xbef4849e);
	CHECK(r->WaveData[155].DestDataSize == 0x800);

	CHECK(r->WaveData[156].crc == 0x7403ad67);
	CHECK(r->WaveData[156].DestDataSize == 0x800);

	CHECK(r->WaveData[157].crc == 0xba501142);
	CHECK(r->WaveData[157].DestDataSize == 0x800);

	CHECK(r->WaveData[158].crc == 0x462fc38f);
	CHECK(r->WaveData[158].DestDataSize == 0x800);

	CHECK(r->WaveData[159].crc == 0xfd372d77);
	CHECK(r->WaveData[159].DestDataSize == 0x800);

	CHECK(r->WaveData[160].crc == 0x8cac2b03);
	CHECK(r->WaveData[160].DestDataSize == 0x800);

	CHECK(r->WaveData[161].crc == 0xe28b9973);
	CHECK(r->WaveData[161].DestDataSize == 0x800);

	CHECK(r->WaveData[162].crc == 0x15dd027f);
	CHECK(r->WaveData[162].DestDataSize == 0x800);

	CHECK(r->WaveData[163].crc == 0x35d82e48);
	CHECK(r->WaveData[163].DestDataSize == 0x800);

	CHECK(r->WaveData[164].crc == 0xfd05261c);
	CHECK(r->WaveData[164].DestDataSize == 0x800);

	CHECK(r->WaveData[165].crc == 0x53ce4dd8);
	CHECK(r->WaveData[165].DestDataSize == 0x800);

	CHECK(r->WaveData[166].crc == 0x263e77f);
	CHECK(r->WaveData[166].DestDataSize == 0x800);

	CHECK(r->WaveData[167].crc == 0x37ed1d25);
	CHECK(r->WaveData[167].DestDataSize == 0x800);

	CHECK(r->WaveData[168].crc == 0xb10feb4a);
	CHECK(r->WaveData[168].DestDataSize == 0x800);

	CHECK(r->WaveData[169].crc == 0x1e331281);
	CHECK(r->WaveData[169].DestDataSize == 0x800);

	CHECK(r->WaveData[170].crc == 0xd55108e4);
	CHECK(r->WaveData[170].DestDataSize == 0x800);

	CHECK(r->WaveData[171].crc == 0x96395860);
	CHECK(r->WaveData[171].DestDataSize == 0x800);

	CHECK(r->WaveData[172].crc == 0xa7f71309);
	CHECK(r->WaveData[172].DestDataSize == 0x800);

	CHECK(r->WaveData[173].crc == 0xe7c9ce6e);
	CHECK(r->WaveData[173].DestDataSize == 0x800);

	CHECK(r->WaveData[174].crc == 0xd05c6497);
	CHECK(r->WaveData[174].DestDataSize == 0x800);

	CHECK(r->WaveData[175].crc == 0xba971cd8);
	CHECK(r->WaveData[175].DestDataSize == 0x800);

	CHECK(r->WaveData[176].crc == 0xdc4d5468);
	CHECK(r->WaveData[176].DestDataSize == 0x800);

	CHECK(r->WaveData[177].crc == 0x87246409);
	CHECK(r->WaveData[177].DestDataSize == 0x800);

	CHECK(r->WaveData[178].crc == 0xcadbb206);
	CHECK(r->WaveData[178].DestDataSize == 0x800);

	CHECK(r->WaveData[179].crc == 0x3318750);
	CHECK(r->WaveData[179].DestDataSize == 0x800);

	CHECK(r->WaveData[180].crc == 0xbe138ebf);
	CHECK(r->WaveData[180].DestDataSize == 0x800);

	CHECK(r->WaveData[181].crc == 0x6978adea);
	CHECK(r->WaveData[181].DestDataSize == 0x800);

	CHECK(r->WaveData[182].crc == 0x3219d559);
	CHECK(r->WaveData[182].DestDataSize == 0x800);

	CHECK(r->WaveData[183].crc == 0x3224541e);
	CHECK(r->WaveData[183].DestDataSize == 0x800);

	CHECK(r->WaveData[184].crc == 0xfd684c17);
	CHECK(r->WaveData[184].DestDataSize == 0x800);

	CHECK(r->WaveData[185].crc == 0xf599d2cf);
	CHECK(r->WaveData[185].DestDataSize == 0x800);

	CHECK(r->WaveData[186].crc == 0x2551a1c7);
	CHECK(r->WaveData[186].DestDataSize == 0x800);

	CHECK(r->WaveData[187].crc == 0x1aa06f92);
	CHECK(r->WaveData[187].DestDataSize == 0x800);

	CHECK(r->WaveData[188].crc == 0xdc04c110);
	CHECK(r->WaveData[188].DestDataSize == 0x800);

	CHECK(r->WaveData[189].crc == 0xb5f3f9e5);
	CHECK(r->WaveData[189].DestDataSize == 0x800);

	CHECK(r->WaveData[190].crc == 0x38c31d54);
	CHECK(r->WaveData[190].DestDataSize == 0x800);

	CHECK(r->WaveData[191].crc == 0x27ecf35c);
	CHECK(r->WaveData[191].DestDataSize == 0x800);

	CHECK(r->WaveData[192].crc == 0x44de5786);
	CHECK(r->WaveData[192].DestDataSize == 0x800);

	CHECK(r->WaveData[193].crc == 0x478ade88);
	CHECK(r->WaveData[193].DestDataSize == 0x800);

	CHECK(r->WaveData[194].crc == 0xf53dc54f);
	CHECK(r->WaveData[194].DestDataSize == 0x800);

	CHECK(r->WaveData[195].crc == 0x2fd9fcf8);
	CHECK(r->WaveData[195].DestDataSize == 0x800);

	CHECK(r->WaveData[196].crc == 0xd516ed7);
	CHECK(r->WaveData[196].DestDataSize == 0x800);

	CHECK(r->WaveData[197].crc == 0x191db40c);
	CHECK(r->WaveData[197].DestDataSize == 0x800);

	CHECK(r->WaveData[198].crc == 0xbe90d2e0);
	CHECK(r->WaveData[198].DestDataSize == 0x800);

	CHECK(r->WaveData[199].crc == 0xff95f300);
	CHECK(r->WaveData[199].DestDataSize == 0x800);

	CHECK(r->WaveData[200].crc == 0xf53d6f4f);
	CHECK(r->WaveData[200].DestDataSize == 0x800);

	CHECK(r->WaveData[201].crc == 0x2e885fb8);
	CHECK(r->WaveData[201].DestDataSize == 0x800);

	CHECK(r->WaveData[202].crc == 0xf6475de8);
	CHECK(r->WaveData[202].DestDataSize == 0x800);

	CHECK(r->WaveData[203].crc == 0x96eaead6);
	CHECK(r->WaveData[203].DestDataSize == 0x800);

	CHECK(r->WaveData[204].crc == 0x1fc9eaa1);
	CHECK(r->WaveData[204].DestDataSize == 0x800);

	CHECK(r->WaveData[205].crc == 0xd7e7009d);
	CHECK(r->WaveData[205].DestDataSize == 0x800);

	CHECK(r->WaveData[206].crc == 0xa083f765);
	CHECK(r->WaveData[206].DestDataSize == 0x800);

	CHECK(r->WaveData[207].crc == 0x29c51423);
	CHECK(r->WaveData[207].DestDataSize == 0x800);

	CHECK(r->WaveData[208].crc == 0xecc355b0);
	CHECK(r->WaveData[208].DestDataSize == 0x800);

	CHECK(r->WaveData[209].crc == 0x3c81e2bf);
	CHECK(r->WaveData[209].DestDataSize == 0x800);

	CHECK(r->WaveData[210].crc == 0xf3bbf7b2);
	CHECK(r->WaveData[210].DestDataSize == 0x800);

	CHECK(r->WaveData[211].crc == 0x88adb9f8);
	CHECK(r->WaveData[211].DestDataSize == 0x800);

	CHECK(r->WaveData[212].crc == 0x82745be5);
	CHECK(r->WaveData[212].DestDataSize == 0x800);

	CHECK(r->WaveData[213].crc == 0x8268458);
	CHECK(r->WaveData[213].DestDataSize == 0x800);

	CHECK(r->WaveData[214].crc == 0x64e1c20b);
	CHECK(r->WaveData[214].DestDataSize == 0x800);

	CHECK(r->WaveData[215].crc == 0x8f13d2c4);
	CHECK(r->WaveData[215].DestDataSize == 0x800);

	CHECK(r->WaveData[216].crc == 0x1b93927d);
	CHECK(r->WaveData[216].DestDataSize == 0x800);

	CHECK(r->WaveData[217].crc == 0xe0b2fc7f);
	CHECK(r->WaveData[217].DestDataSize == 0x800);

	CHECK(r->WaveData[218].crc == 0xb37bc56c);
	CHECK(r->WaveData[218].DestDataSize == 0x800);

	CHECK(r->WaveData[219].crc == 0x910dfcb8);
	CHECK(r->WaveData[219].DestDataSize == 0x800);

	CHECK(r->WaveData[220].crc == 0xce04f4a8);
	CHECK(r->WaveData[220].DestDataSize == 0x800);

	CHECK(r->WaveData[221].crc == 0xa953aeca);
	CHECK(r->WaveData[221].DestDataSize == 0x800);

	CHECK(r->WaveData[222].crc == 0xc11e3316);
	CHECK(r->WaveData[222].DestDataSize == 0x800);

	CHECK(r->WaveData[223].crc == 0x3ead8276);
	CHECK(r->WaveData[223].DestDataSize == 0x800);

	CHECK(r->WaveData[224].crc == 0x6d3e2277);
	CHECK(r->WaveData[224].DestDataSize == 0x800);

	CHECK(r->WaveData[225].crc == 0x8495017f);
	CHECK(r->WaveData[225].DestDataSize == 0x800);

	CHECK(r->WaveData[226].crc == 0xc2a682e2);
	CHECK(r->WaveData[226].DestDataSize == 0x800);

	CHECK(r->WaveData[227].crc == 0xe7eb172a);
	CHECK(r->WaveData[227].DestDataSize == 0x800);

	CHECK(r->WaveData[228].crc == 0x608ca32a);
	CHECK(r->WaveData[228].DestDataSize == 0x800);

	CHECK(r->WaveData[229].crc == 0xb801103f);
	CHECK(r->WaveData[229].DestDataSize == 0x800);

	CHECK(r->WaveData[230].crc == 0x727dc765);
	CHECK(r->WaveData[230].DestDataSize == 0x800);

	CHECK(r->WaveData[231].crc == 0xb21c5af0);
	CHECK(r->WaveData[231].DestDataSize == 0x800);

	CHECK(r->WaveData[232].crc == 0x9996fad3);
	CHECK(r->WaveData[232].DestDataSize == 0x800);

	CHECK(r->WaveData[233].crc == 0x1ca2d897);
	CHECK(r->WaveData[233].DestDataSize == 0x800);

	CHECK(r->WaveData[234].crc == 0x7cf184f7);
	CHECK(r->WaveData[234].DestDataSize == 0x800);

	CHECK(r->WaveData[235].crc == 0x69a5f163);
	CHECK(r->WaveData[235].DestDataSize == 0x800);

	CHECK(r->WaveData[236].crc == 0x2aff1f14);
	CHECK(r->WaveData[236].DestDataSize == 0x800);

	CHECK(r->WaveData[237].crc == 0x4b557b84);
	CHECK(r->WaveData[237].DestDataSize == 0x800);

	CHECK(r->WaveData[238].crc == 0xa48056de);
	CHECK(r->WaveData[238].DestDataSize == 0x800);

	CHECK(r->WaveData[239].crc == 0x9ca7a983);
	CHECK(r->WaveData[239].DestDataSize == 0x800);

	CHECK(r->WaveData[240].crc == 0x9480c698);
	CHECK(r->WaveData[240].DestDataSize == 0x800);

	CHECK(r->WaveData[241].crc == 0xe94158c6);
	CHECK(r->WaveData[241].DestDataSize == 0x800);

	CHECK(r->WaveData[242].crc == 0x2c30302a);
	CHECK(r->WaveData[242].DestDataSize == 0x800);

	CHECK(r->WaveData[243].crc == 0x2f7c973d);
	CHECK(r->WaveData[243].DestDataSize == 0x800);

	CHECK(r->WaveData[244].crc == 0xcf484a57);
	CHECK(r->WaveData[244].DestDataSize == 0x800);

	CHECK(r->WaveData[245].crc == 0x30adce8c);
	CHECK(r->WaveData[245].DestDataSize == 0x800);

	CHECK(r->WaveData[246].crc == 0x7ecb037d);
	CHECK(r->WaveData[246].DestDataSize == 0x800);

	CHECK(r->WaveData[247].crc == 0xec12e74c);
	CHECK(r->WaveData[247].DestDataSize == 0x800);

	CHECK(r->WaveData[248].crc == 0xb5fda755);
	CHECK(r->WaveData[248].DestDataSize == 0x800);

	CHECK(r->WaveData[249].crc == 0xc54c56cf);
	CHECK(r->WaveData[249].DestDataSize == 0x800);

	CHECK(r->WaveData[250].crc == 0xee408f43);
	CHECK(r->WaveData[250].DestDataSize == 0x800);

	CHECK(r->WaveData[251].crc == 0xa15c88c6);
	CHECK(r->WaveData[251].DestDataSize == 0x800);

	CHECK(r->WaveData[252].crc == 0xec3dc9fc);
	CHECK(r->WaveData[252].DestDataSize == 0x800);

	CHECK(r->WaveData[253].crc == 0x16ffc74a);
	CHECK(r->WaveData[253].DestDataSize == 0x800);

	CHECK(r->WaveData[254].crc == 0x1acf4e43);
	CHECK(r->WaveData[254].DestDataSize == 0x800);

	CHECK(r->WaveData[255].crc == 0x2943dbb6);
	CHECK(r->WaveData[255].DestDataSize == 0x800);

#endif
} TEST_END

TEST_TEARDOWN(Wave_Test)
{
#if Wave_Test



#endif
}

// --- End of File ---

