//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "KillableBase.h"

// ----------------------------------
// Static initializations
// ----------------------------------
int KillableBase::ThreadCount::count = 0;
KillableBase::KillShare *KillableBase::posKillShare = nullptr;
KillableBase::ThreadCount::ThreadCountDone *KillableBase::ThreadCount::posThreadCountDone = nullptr;

// ----------------------------------
// ThreadCount
// ----------------------------------
void KillableBase::ThreadCount::WaitUntilThreadDone()
{
	std::unique_lock<std::mutex> lock_ct(posThreadCountDone->mtx);
	posThreadCountDone->cv.wait(lock_ct);
}

KillableBase::ThreadCount::ThreadCount(const char *pName)
	:mtx()
{
	std::lock_guard<std::mutex> lock(mtx);
	count++;
     Debug::out("TC:%d (%s)\n", count, pName);

	if(posThreadCountDone == nullptr)
	{
		posThreadCountDone = new ThreadCountDone();
		assert(posThreadCountDone);
	}
	assert(posThreadCountDone);
}

KillableBase::ThreadCount::~ThreadCount()
{
	std::lock_guard<std::mutex> lock(mtx);
	count--;

     Debug::out("~TC:%d \n", count);

	if(count == 1)
	{
		posThreadCountDone->cv.notify_one();
		delete KillableBase::posKillShare;
		KillableBase::posKillShare = nullptr;
	}
	if(count == 0)
	{
		delete posThreadCountDone;
		posThreadCountDone = nullptr;
	}
}

// ----------------------------------
// KillableBase
// ----------------------------------
KillableBase::KillableBase(const char *const pName)
	: BannerBase(pName),
	TC(pName)
{
	assert(pName);

	if(posKillShare == nullptr)
	{
		posKillShare = new KillableBase::KillShare();
		assert(posKillShare);
	}
	assert(posKillShare);
}

bool KillableBase::KillEvent()
{
	// this is your exit condition - keep at 0s
	return (KillableBase::posKillShare->fu.wait_for(0s) == std::future_status::ready);
}

KillableBase::KillShare::KillShare()
	: prom(),
	fu(prom.get_future())
{
}

// --- End of File ---
