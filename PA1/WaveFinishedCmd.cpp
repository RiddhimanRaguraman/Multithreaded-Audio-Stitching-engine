//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "WaveFinishedCmd.h"
#include "PlaybackThread.h"

WaveFinishedCmd::WaveFinishedCmd(int idx)
    : index(idx)
{
}

void WaveFinishedCmd::Execute()
{
    // Notify Playcoordshare
    PlaybackThread::OnWaveFinished(this->index);
    delete this;
}

// --- End of File ---