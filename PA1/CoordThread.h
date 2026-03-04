//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef COORD_THREAD_H
#define COORD_THREAD_H

#include "KillableBase.h"
#include "FileCoordShare.h"
#include "Buffer.h"
#include "CircularData.h"

#include "CoordState.h"

class CoordThread : public KillableBase
{
public:
    static const unsigned int BUFFER_SIZE = 512 * 1024;

public:
    CoordThread(const char *const pName,
                FileCoordShare &share_fc,
                CircularData &CoordInQueue);

	CoordThread() = delete;
	CoordThread(const CoordThread &) = delete;
	CoordThread &operator = (const CoordThread &) = delete;
	~CoordThread();

    void Launch();

    void operator()();

    static size_t CopyNext2KTo(unsigned char *pDest, size_t destSize);

    // Quiescence helpers for pipeline status (no new queues)
    static size_t DrainRemaining();   // Remaining bytes across front/back
    static bool   IsDraining();       // True when combined remaining > 0

    // First-drain barrier (one-shot) to coordinate startup ordering
    static void WaitForFirstDrain();

    // Event notifier called by Playback after enqueuing WaveFillCmds
    static void NotifyCoordWork();

private:
    void privSwapBuffers();

    void SetState(CoordState* s) { this->mState = s; }

    // --------------------------
    // Data
    // --------------------------

    std::thread mThread;

    FileCoordShare &share_fc;

    Buffer *pBuff_A;
    Buffer *pBuff_B;

    // Front/back roles for playback coordination
    Buffer *pFront;        // current front (being played)
    Buffer *pBack;         // current back  (being filled)

    // Minimal coordinated positions (always frame-aligned, multiples of 4)
    size_t  frontPos;      // consumed bytes in pFront
    size_t  backPos;       // consumed bytes in pBack

    CircularData &inQueue;

    // Event CV used to wake coordinator on work (file ready OR fills posted)
    std::mutex evtMtx;
    std::condition_variable evtCv;
    bool coordHasWork;

    unsigned char pad[7];

    // Coordinator state mutex (guards front/back counters and pointers)
    std::mutex coordMtx;

    // Singleton for static access from command execution
    static CoordThread *sInstance;
public:
    static CoordThread* Instance();
private:

    // One-shot barrier state to notify Playback of first drain readiness
    static std::mutex sFirstDrainMtx;
    static std::condition_variable sFirstDrainCv;
    static bool sFirstDrainReady;
    static void SignalFirstDrain();

    // Current state pointer
    CoordState* mState{nullptr};

    // Allow states to access internals for performance and minimal API surface
    friend class CoordInitState;
    friend class CoordPlayState;
    friend class CoordEndingState;
    friend class CoordFinishedState;
};

#endif

// --- End of File ---
