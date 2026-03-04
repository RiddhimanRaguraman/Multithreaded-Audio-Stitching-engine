//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include <conio.h>

#include "KillThread.h"
#include "PlaybackThread.h"
#include "FileThread.h"
#include "UnitTestThread.h"
#include "CoordThread.h"

KillThread* KillThread::sInstance = nullptr;
bool KillThread::hasSignaled = false;
std::mutex KillThread::signalMtx;


KillThread::KillThread(const char *const pName)
	: KillableBase(pName),
	mThread()
{
	// ThreadCount
    KillThread::sInstance = this;
}

void KillThread::SignalThreadsToDie()
{
    // Wait until playback has fully completed before signaling kill
    std::mutex wait_mtx;
    std::unique_lock<std::mutex> wait_lock(wait_mtx);
    std::condition_variable wait_cv;
    while (!wait_cv.wait_for( wait_lock,
                std::chrono::milliseconds(0s),
                [] {
                    return PlaybackThread::IsPlaybackCompleted();
                }))
    {
        // keep waiting until predicate is true
    }

    // Signal once (set the promise exactly once, after completion)
    {
        std::lock_guard<std::mutex> lk(KillThread::signalMtx);
        if (!KillThread::hasSignaled)
        {
            KillableBase::posKillShare->prom.set_value();
            KillThread::hasSignaled = true;

            // Wake Coordinator's event-driven wait so it can observe KillEvent()
            CoordThread::NotifyCoordWork();
        }
    }
}

void KillThread::operator()()
{
    START_BANNER;

    // Do not rely on keypress; wait until work is complete then signal
    this->SignalThreadsToDie();

    TC.WaitUntilThreadDone();
}

KillThread::~KillThread()
{
	if(this->mThread.joinable())
	{
		this->mThread.join();
		Debug::out("~KillThread()  join \n");
	}
}

void KillThread::Launch()
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

void KillThread::SignalAllThreadsToDie()
{
    KillThread* inst = KillThread::Instance();
    if (inst != nullptr)
    {
        inst->SignalThreadsToDie();
    }
}

KillThread* KillThread::Instance()
{
    return KillThread::sInstance;
}

// --- End of File ---
