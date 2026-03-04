//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef PLAY_COORD_SHARE_H
#define PLAY_COORD_SHARE_H

#include "CircularData.h"
#include "UnitTestThread.h"

class WaveBuffer;

class PlayCoordShare
{
public:
    static PlayCoordShare &Instance();

    void RegisterCoordQueue(CircularData *q);
    void RegisterWave(int idx, WaveBuffer *p);
    void UnregisterWave(int idx);

    void MarkReady(int idx);
    void WaitUntilAllReady(int count);

    // Per-buffer readiness helpers
    bool IsReady(int idx);
    void WaitForReady(int idx);
    bool ConsumeReady(int idx);

    // End-of-stream drain tracking
    void NotifyDrain();
    int  GetDrains();

    // In-flight playback tracking
    void IncrementPending();
    void DecrementPending();
    int  GetPending();
      
private:
    PlayCoordShare();

    WaveBuffer *waves[UnitTestThread::WAVE_COUNT];
    bool        ready[UnitTestThread::WAVE_COUNT];
    int readyCount;
    CircularData *coordQueue;
    std::mutex mtx;
    std::condition_variable cv;
    int drains;
    int pending;
};

#endif

// --- End of File ---