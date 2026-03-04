//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "WaveBuffer.h"
#include "Command.h"
#include "CircularData.h"
#include "WaveFinishedCmd.h"
#include "PlayCoordShare.h"
#include "UnitTestThread.h"


HWAVEOUT WaveBuffer::hWaveOut = nullptr;
CircularData *WaveBuffer::playbackQueue = nullptr;
std::mutex WaveBuffer::sDeviceMtx;

void WaveBuffer::SetPlaybackQueue(CircularData *q)
{
    WaveBuffer::playbackQueue = q;
}

void WaveBuffer::NotifyPlayback(Command *pCmd)
{
    if(WaveBuffer::playbackQueue && pCmd)
    {
        WaveBuffer::playbackQueue->PushBack(pCmd);
    }
}

WaveBuffer::WaveBuffer(int idx)
    : KillableBase("--WBUF---"), 
    mtx(), 
    cv(), 
    pBuff(new unsigned char[2048]), 
    buffSize(2048), 
    mThread(), 
    index(idx), 
    status(BufferStatus::ReadyToRead),
    requestPlay(false)
{
    std::memset(pBuff, 0, buffSize);
}

WaveBuffer::~WaveBuffer()
{
    if (mThread.joinable())
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            requestPlay = true; 
        }
        cv.notify_one();
        mThread.join();
    }
    delete[] pBuff;
}

void WaveBuffer::Launch()
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

void WaveBuffer::RequestPlay()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        requestPlay = true;
    }
    cv.notify_one();
}

void WaveBuffer::operator()()
{
    while (!KillEvent())
    {
        std::unique_lock<std::mutex> lock(mtx);
        bool ready = cv.wait_for(lock, 0ms,
            [this]()
            {
                return this->requestPlay || KillEvent();
            });
        if (KillEvent()) { break; }
        if (!ready)
        {
            lock.unlock();
            std::this_thread::yield();
            continue;
        }
        
        requestPlay = false;
        lock.unlock();
        PlayNow();
    }
}

WaveBuffer::BufferStatus WaveBuffer::GetStatus()
{
    std::lock_guard<std::mutex> lk(mtx);
    return status;
}

void WaveBuffer::SetStatus(BufferStatus s)
{
    std::lock_guard<std::mutex> lk(mtx);
    status = s;
}

void WaveBuffer::PlayNow()
{
    if (!WaveBuffer::hWaveOut)
        return;
    {
        std::lock_guard<std::mutex> lk(WaveBuffer::sDeviceMtx);
        WAVEHDR *pHdr = new WAVEHDR{};
        pHdr->lpData = reinterpret_cast<LPSTR>(this->pBuff);
        pHdr->dwBufferLength = static_cast<DWORD>(this->buffSize);
        pHdr->dwFlags = 0;
        pHdr->dwUser = static_cast<DWORD_PTR>(this->index);
        MMRESULT mm = ::waveOutPrepareHeader(WaveBuffer::hWaveOut, pHdr, sizeof(WAVEHDR));
        assert(mm == MMSYSERR_NOERROR);
        mm = ::waveOutWrite(WaveBuffer::hWaveOut, pHdr, sizeof(WAVEHDR));
        assert(mm == MMSYSERR_NOERROR);
        PlayCoordShare::Instance().IncrementPending();
    }
    
}

void CALLBACK WaveBuffer::WaveOutProc(HWAVEOUT hwo, UINT uMsg,
                                      DWORD_PTR dwInstance,
                                      DWORD_PTR dwParam1,
                                      DWORD_PTR dwParam2)
{
    // Use the wave-out "done" message explicitly
    if (uMsg == MM_WOM_DONE) {

        WAVEHDR* pHdr = reinterpret_cast<WAVEHDR*>(dwParam1);
        const int finishedIdx = static_cast<int>(pHdr->dwUser);

        MMRESULT mm = ::waveOutUnprepareHeader(hwo, pHdr, sizeof(WAVEHDR));
        assert(mm == MMSYSERR_NOERROR);
        (void)mm;
        if (WaveBuffer::playbackQueue)
        {
            // Debug::out("\t waveOutDone idx=%d\n", finishedIdx);
            Command* pCmd = new WaveFinishedCmd(finishedIdx);
            WaveBuffer::playbackQueue->PushBack(pCmd);
        }

        delete pHdr;
        PlayCoordShare::Instance().DecrementPending();
    }
}

// --- End of File ---
