//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef KILL_THREAD_H
#define KILL_THREAD_H

#include "KillableBase.h"

class KillThread : public KillableBase
{
public:

	KillThread(const char *const pName);

	KillThread() = delete;
	KillThread(const KillThread &) = delete;
	KillThread &operator = (const KillThread &) = delete;
	~KillThread();

	void SignalThreadsToDie();
	void Launch();

	void operator()();

    static void SignalAllThreadsToDie();
    static KillThread* Instance();

private:
    static KillThread* sInstance;
    static bool hasSignaled;
    static std::mutex signalMtx;
    std::thread mThread;

};

#endif

// --- End of File ---
