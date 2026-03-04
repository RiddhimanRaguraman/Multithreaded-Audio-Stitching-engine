//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "File.h"
#include "KillThread.h"
#include "FileThread.h"
#include "CoordThread.h"
#include "FileCoordShare.h"
#include "PlaybackThread.h"
#include "PlayCoordShare.h"
#include "WaveBuffer.h"
#include "UnitTestThread.h"
#include "CircularData.h"

using namespace Azul;

int main()
{
	START_BANNER_MAIN("-Main-");

	File::Error error;
	uint32_t MagicNum;
	error = File::Magic(MagicNum);
	assert(error == File::Error::SUCCESS);
	assert(MagicNum == 0xA4B1);
    // Debug::out("File::SUCCESS\n");

	// ----------------------------------------------------------------
	// setup objects
	//-----------------------------------------------------------------
	FileCoordShare share_fc;
	CircularData CoordInQueue;

    KillThread tKill("--KILL---");
    UnitTestThread tUnitTest("--UNIT---");
    FileThread tFile("--FILE---", share_fc);
    CoordThread tCoord("--COORD--", share_fc, CoordInQueue);
    PlaybackThread tPlayback("--PLAY---", CoordInQueue);

	tKill.Launch();
	tUnitTest.Launch();
	tFile.Launch();
	tCoord.Launch();
	tPlayback.Launch();

	// Automatic shutdown: KillThread will signal when all work completes


}

// ---  End of File ---
