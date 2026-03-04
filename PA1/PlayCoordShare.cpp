//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "PlayCoordShare.h"
#include "WaveBuffer.h"

PlayCoordShare &PlayCoordShare::Instance()
{
    static PlayCoordShare instance;
    return instance;
}

PlayCoordShare::PlayCoordShare()
    : waves{}, 
    ready{}, 
    readyCount(0), 
    coordQueue(nullptr), 
    drains(0), 
    pending(0)
{
    for (int i = 0; i < (int)UnitTestThread::WAVE_COUNT; ++i)
    {
        waves[i] = nullptr;
        ready[i] = false;
    }
}

void PlayCoordShare::RegisterCoordQueue(CircularData *q)
{
    std::lock_guard<std::mutex> lock(mtx);
    coordQueue = q;
}

void PlayCoordShare::RegisterWave(int idx, WaveBuffer *p)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (idx >= 0 && idx < (int)UnitTestThread::WAVE_COUNT) waves[idx] = p;
}

void PlayCoordShare::UnregisterWave(int idx)
{
    if (idx < 0 || idx >= (int)UnitTestThread::WAVE_COUNT)
        return;

    std::lock_guard<std::mutex> lock(mtx);
    waves[idx] = nullptr;
    ready[idx] = false;
}

void PlayCoordShare::MarkReady(int idx)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (idx >= 0 && idx < (int)UnitTestThread::WAVE_COUNT && !ready[idx])
    {
        ready[idx] = true;
        ++readyCount;
        cv.notify_all();
    }
}

void PlayCoordShare::WaitUntilAllReady(int count)
{
    std::unique_lock<std::mutex> lk(mtx);
    (void)cv.wait_for(lk, 0ms, [&]{ return this->readyCount >= count; });
}

void PlayCoordShare::NotifyDrain()
{
    std::lock_guard<std::mutex> lk(mtx);
    ++drains;
}

void PlayCoordShare::IncrementPending()
{
    std::lock_guard<std::mutex> lk(mtx);
    ++pending;
}

void PlayCoordShare::DecrementPending()
{
    std::lock_guard<std::mutex> lk(mtx);
    --pending;
}

int PlayCoordShare::GetDrains()
{
    std::lock_guard<std::mutex> lk(mtx);
    return drains;
}

int PlayCoordShare::GetPending()
{
    std::lock_guard<std::mutex> lk(mtx);
    return pending;
}

bool PlayCoordShare::IsReady(int idx)
{
    std::lock_guard<std::mutex> lk(mtx);
    return (idx >= 0 && idx < (int)UnitTestThread::WAVE_COUNT) ? ready[idx] : false;
}

void PlayCoordShare::WaitForReady(int idx)
{
    std::unique_lock<std::mutex> lk(mtx);
    (void)cv.wait_for(lk, 0ms, [&]{
        return (idx >= 0 && idx < (int)UnitTestThread::WAVE_COUNT) && ready[idx];
    });
}

bool PlayCoordShare::ConsumeReady(int idx)
{
    std::lock_guard<std::mutex> lk(mtx);
    if (idx < 0 || idx >= (int)UnitTestThread::WAVE_COUNT) return false;
    if (!ready[idx]) return false;
    ready[idx] = false;
    return true;
}

// --- End of File ---