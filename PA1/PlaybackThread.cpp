//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "PlaybackThread.h"
#include "WaveFillCmd.h"
#include "UnitTestThread.h"
#include "FileThread.h"
#include "WaveFinishedCmd.h"
#include "PlayCoordShare.h"
#include "WaveBuffer.h"
#include "CoordThread.h"
#include "Command.h"


bool PlaybackThread::playbackCompleted = false;
std::mutex PlaybackThread::sMtx;

PlaybackThread* PlaybackThread::sInstance = nullptr;

PlaybackThread* PlaybackThread::Instance()
{
    return PlaybackThread::sInstance;
}

void PlaybackThread::OnWaveFinished(int idx)
{
    PlaybackThread* inst = PlaybackThread::Instance();
    if (inst)
    {
        inst->handleFinished(idx);
    }
}


PlaybackThread::PlaybackThread(const char *const pName,
                               CircularData &CoordInQueue)
    : KillableBase(pName),
    mThread(),
    inQueue(CoordInQueue),
    playbackInQueue(),
    preloadsPosted(false)
{
    // Provide callback queue to WaveBuffer for finished notifications
    WaveBuffer::playbackQueue = &this->playbackInQueue;

    // Initialize the shared WaveOut device via WaveBuffer
    this->InitAudioDevice();
  
    // Set static instance for routing finished callbacks to this object
    PlaybackThread::sInstance = this;

    // Acquire pre-created WaveBuffer workers from PlayCoordShare (created in main)
    for(int i = 0; i < (int)UnitTestThread::WAVE_COUNT; ++i)
    {
        WaveBuffer* pW = new WaveBuffer(i);
        UnitTestThread::Wave_BufferInfo(pW->GetBuff(), pW->GetBuffSize(), i);
        PlayCoordShare::Instance().RegisterWave(i, pW);
        pW->Launch();
        waves[i] = pW;
        assert(waves[i] != nullptr);
    }

}



bool PlaybackThread::privPreload()
{
    PlayCoordShare::Instance().RegisterCoordQueue(&inQueue);
    if (!preloadsPosted)
    {
        for (int i = 0; i < (int)UnitTestThread::WAVE_COUNT; ++i)
        {
            inQueue.PushBack(new WaveFillCmd(waves[i], true));
            CoordThread::NotifyCoordWork();
            waves[i]->SetStatus(WaveBuffer::BufferStatus::Filling);
        }
        preloadsPosted = true;
    }
    return (CoordThread::DrainRemaining() > 0);
}

void PlaybackThread::operator()()
{
    START_BANNER;
    // Preload + first-cycle kickoff
    bool prevHasBytes = this->privPreload();

	while(!KillEvent())
	{
        // Event-loop gating per iteration using a single 0ms wait_for
        bool haveWork = false;
        {
            std::unique_lock<std::mutex> lk(this->evtMtx);
            haveWork = this->evtCv.wait_for(
                lk,
                0ms,
                [&]
                {
                    if (KillEvent()) return true;
                    if (!this->playbackInQueue.IsEmpty()) return true;
                    if (PlayCoordShare::Instance().GetPending() > 0) return true;
                    if (CoordThread::IsDraining()) return true;
                    return false;
                });
        }
        if (!haveWork)
        {
            continue;
        }

        // Process all pending playback callbacks (finished buffers or drain notices)
        Command *cbCmd = nullptr;
        while(playbackInQueue.PopFront(cbCmd))
        {
            cbCmd->Execute();
            cbCmd = nullptr;
        }

        // Edge case where all buffers are completed playing
        const bool hasBytes = (CoordThread::DrainRemaining() > 0);
        if (!prevHasBytes && hasBytes)
        {
            int posted = 0;
            const int MAX_POST = 4;
            for (int i = 0; i < (int)UnitTestThread::WAVE_COUNT && posted < MAX_POST; ++i)
            {
                if (waves[i]->GetStatus() == WaveBuffer::BufferStatus::ReadyToRead && !PlayCoordShare::Instance().IsReady(i))
                {
                    inQueue.PushBack(new WaveFillCmd(waves[i], false));
                    CoordThread::NotifyCoordWork();
                    waves[i]->SetStatus(WaveBuffer::BufferStatus::Filling);
                    ++posted;
                }
            }
        }
        prevHasBytes = hasBytes;

        for (int i = 0; i < (int)UnitTestThread::WAVE_COUNT; ++i)
        {
            if (PlayCoordShare::Instance().ConsumeReady(i))
            {
                waves[i]->SetStatus(WaveBuffer::BufferStatus::ReadyToPlay);

                waves[i]->RequestPlay();
                waves[i]->SetStatus(WaveBuffer::BufferStatus::Playing);
            }
        }

        // Ending Logic
        const bool producerDone   = (FileThread::GetFileReadCount() >= FileThread::MAX_NUM_WAVE_FILES);
        const bool noInflight     = (PlayCoordShare::Instance().GetPending() == 0); // check if it is required
        const bool qPlaybackEmpty = playbackInQueue.IsEmpty();
        const bool noCoordBytes   = !CoordThread::IsDraining() || (CoordThread::DrainRemaining() == 0);
   
        if ( producerDone && noInflight && qPlaybackEmpty && noCoordBytes)
        {
            Debug::out("Playback complete: all audio played.\n");
            PlaybackThread::SetPlaybackCompleted(true);
            break;
        }

	}
    
}

PlaybackThread::~PlaybackThread()
{
    // 1) Join the PlaybackThread
    if (this->mThread.joinable())
    {
        this->mThread.join();
        Debug::out("~PlaybackThread() join\n");
    }

    // 2) Stop & delete all WaveBuffer workers (also frees their 2KB buffers)
    for (int i = 0; i < (int)UnitTestThread::WAVE_COUNT; ++i)
    {
        if (waves[i])
        {
            PlayCoordShare::Instance().UnregisterWave(i);
            delete waves[i];           
            waves[i] = nullptr;
        }
    }

    // 3) Reset and close the WaveOut device via WaveBuffer
    this->ShutdownAudioDevice();

    // 4) Drain & delete leftover commands in the playback callback queue
    Command *cmd = nullptr;
    while (playbackInQueue.PopFront(cmd))
    {
        delete cmd; // do NOT Execute() during teardown — just delete
        cmd = nullptr;
    }

}

void PlaybackThread::Launch()
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


void PlaybackThread::handleFinished(int idx)
{
    if(idx < 0) return; 

    // Record the completion
    // Debug::out("\t finished idx=%d\n", idx);

    const bool draining     = CoordThread::IsDraining();
    const bool hasBytes     = (CoordThread::DrainRemaining() > 0);
    const bool producerDone = (FileThread::GetFileReadCount() >= FileThread::MAX_NUM_WAVE_FILES);

    if (producerDone && (!draining || !hasBytes))
    {
        this->waves[idx]->SetStatus(WaveBuffer::BufferStatus::Done);
        return;
    }

    if (hasBytes)
    {
        this->waves[idx]->SetStatus(WaveBuffer::BufferStatus::Filling);
        this->inQueue.PushBack(new WaveFillCmd(this->waves[idx], false));
        CoordThread::NotifyCoordWork();
    }
    else
    {
        this->waves[idx]->SetStatus(WaveBuffer::BufferStatus::ReadyToRead);
    }

    // Note: actual launches occur on the main loop after processing callbacks.
    // We avoid starting bursts here to keep ordering stable and reduce races.
}

bool PlaybackThread::IsPlaybackCompleted()
{
    std::lock_guard<std::mutex> lk(PlaybackThread::sMtx);
    return PlaybackThread::playbackCompleted;
}

void PlaybackThread::SetPlaybackCompleted(bool v)
{
    std::lock_guard<std::mutex> lk(PlaybackThread::sMtx);
    PlaybackThread::playbackCompleted = v;
}

void PlaybackThread::InitAudioDevice()
{
    if (WaveBuffer::hWaveOut)
        return;

    // Configure WaveOut device: 22,050 Hz, 16-bit, stereo PCM
    WAVEFORMATEX wfx{};
    wfx.wFormatTag = static_cast<WORD>(WAVE_FORMAT_PCM);
    wfx.nChannels = static_cast<WORD>(2);
    wfx.nSamplesPerSec = 22050;
    wfx.wBitsPerSample = static_cast<WORD>(16);
    wfx.nBlockAlign = static_cast<WORD>((wfx.nChannels * wfx.wBitsPerSample) / 8);
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize = 0;

    MMRESULT res = ::waveOutOpen(&WaveBuffer::hWaveOut,
        WAVE_MAPPER,
        &wfx,
        (DWORD_PTR)&WaveBuffer::WaveOutProc,
        (DWORD_PTR)nullptr,
        CALLBACK_FUNCTION);
    assert(res == MMSYSERR_NOERROR);
    (void)res;
}

void PlaybackThread::ShutdownAudioDevice()
{
    if (WaveBuffer::hWaveOut)
    {
        // Return any outstanding headers and fire WOM_DONE callbacks
        ::waveOutReset(WaveBuffer::hWaveOut);
        ::waveOutClose(WaveBuffer::hWaveOut);
        WaveBuffer::hWaveOut = nullptr;
    }
}