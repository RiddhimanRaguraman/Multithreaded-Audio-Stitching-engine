//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef KILLABLE_BASE_H
#define KILLABLE_BASE_H

class KillableBase : public BannerBase
{
public:

	class ThreadCount
	{
	public:
		struct ThreadCountDone
		{
			std::mutex				 mtx;
			std::condition_variable  cv;
		};

	public:
		ThreadCount(const char *pName);
		ThreadCount() = delete;
		ThreadCount(const ThreadCount &) = delete;
		ThreadCount &operator = (const ThreadCount &) = delete;
		~ThreadCount();

		void WaitUntilThreadDone();

	private:
		// --------------------------
		// Data
		// --------------------------
		static int count;
		std::mutex mtx;

		static ThreadCountDone *posThreadCountDone;
	};

	struct KillShare
	{
		KillShare();
		KillShare(const KillShare &) = delete;
		KillShare &operator=(const KillShare &) = delete;
		~KillShare() = default;

		// --------------------------
		// Data
		// --------------------------
		std::promise<void> prom;
		std::future<void>  fu;
	};

public:
	KillableBase(const char *const pName);

	KillableBase() = delete;
	KillableBase(const KillableBase &) = delete;
	KillableBase &operator = (const KillableBase &) = delete;
	virtual ~KillableBase() = default;

protected:
	bool KillEvent();
	static KillShare *posKillShare;

	// --------------------------
	// Data
	// --------------------------
	ThreadCount TC;
};

#endif

// --- End of File ---
