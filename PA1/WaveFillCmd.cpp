// ----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "WaveFillCmd.h"
#include "CoordThread.h"
#include "WaveBuffer.h"
#include "WaveFinishedCmd.h"
#include "PlayCoordShare.h"

using namespace ThreadFramework;

WaveFillCmd::WaveFillCmd(WaveBuffer *p, bool preload)
    : pWave(p)
    , isPreload(preload)
{
    //assert(p);
}


void WaveFillCmd::Execute()
{
    // Copy a single 2KB chunk
    if (!this->pWave) { delete this; return; }

    size_t wrote = CoordThread::CopyNext2KTo(this->pWave->GetBuff(), this->pWave->GetBuffSize());
    if(wrote > 0)
    {
        // Debug::out("\t  Coord->Wave transfer: idx=%d wrote=%zu bytes\n", this->pWave->GetIndex(), wrote);
        // Mark ready; PlaybackThread will trigger RequestPlay()
        PlayCoordShare::Instance().MarkReady(this->pWave->GetIndex());
    }

    // when done... delete itself
    delete this;
}


// --- End of File ---
