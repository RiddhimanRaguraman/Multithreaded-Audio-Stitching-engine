//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef WAVE_BUFFER_H
#define WAVE_BUFFER_H

#pragma comment(lib, "winmm.lib")

#include "UnitTestThread.h"
#include "CircularData.h"
#include "KillableBase.h"

// Forward declaration to avoid including Command in header
class Command;

class WaveBuffer : public KillableBase
{
public:
    static void SetPlaybackQueue(CircularData *q);
    static void NotifyPlayback(Command *pCmd);

    // Expose device/queue for assignment’s specified wiring
    static HWAVEOUT hWaveOut;
    static CircularData *playbackQueue;

public:
    explicit WaveBuffer(int idx);
    WaveBuffer() = delete;
    WaveBuffer(const WaveBuffer &) = delete;
    WaveBuffer &operator=(const WaveBuffer &) = delete;
    ~WaveBuffer();

    void Launch();
    void RequestPlay();

    // Thread entry point must be public for std::thread(std::ref(*this))
    void operator()();

    unsigned char *GetBuff() const { return pBuff; }
    size_t GetBuffSize() const { return buffSize; }
    int GetIndex() const { return index; }
    enum class BufferStatus { ReadyToRead, Filling, ReadyToPlay, Playing, Done };
    BufferStatus GetStatus();
    void SetStatus(BufferStatus s);
    // static callback bridge
    static void CALLBACK WaveOutProc(HWAVEOUT hwo, UINT uMsg,
        DWORD_PTR dwInstance,
        DWORD_PTR dwParam1,
        DWORD_PTR dwParam2);
private:
    void PlayNow();

private:
    static std::mutex sDeviceMtx;

    // 8-byte aligned / large members FIRST
    std::mutex mtx;
    std::condition_variable cv;
    unsigned char *pBuff;
    size_t         buffSize;
    std::thread    mThread;

    // 4/1-byte members LAST (no 8-byte members must follow these)
    int  index;
    BufferStatus status;
    bool requestPlay;
    unsigned char pad[7]; 
    


};

#endif

// --- End of File ---
