//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef PLAYBACK_THREAD_H
#define PLAYBACK_THREAD_H

#include "KillableBase.h"
#include "CircularData.h"
#include "WaveBuffer.h"

class PlaybackThread : public KillableBase
{
public:
    static const unsigned int BUFFER_SIZE = 512 * 1024;

public:
	PlaybackThread(const char *const pName,
				   CircularData &CoordInQueue);

	PlaybackThread() = delete;
	PlaybackThread(const PlaybackThread &) = delete;
	PlaybackThread &operator = (const PlaybackThread &) = delete;
	~PlaybackThread();

    void Launch();

    void operator()();

    // Public accessor for completion state (no RTTI required)
    static bool IsPlaybackCompleted();
    static void SetPlaybackCompleted(bool v);

    static void InitAudioDevice();
    static void ShutdownAudioDevice();

    // Callback entry point for finished buffers (called from WaveFinishedCmd)
    static void OnWaveFinished(int idx);

private:

    // Thread and coordination state
    // - Owns the playback worker thread and two input queues:
    //   1) Coordinator input queue (commands to request 2KB refills)
    //   2) Playback callback queue (finished notifications from WaveOut)

    std::thread mThread;

    // Reference for queue in coordinate thread
    CircularData &inQueue;

    // Playback thread local queue (fed by WaveBuffer callbacks)
    CircularData playbackInQueue;

    WaveBuffer* waves[UnitTestThread::WAVE_COUNT];

    // Static instance used to route WaveFinishedCmd callback notifications
    static PlaybackThread* sInstance;
public:
    static PlaybackThread* Instance();
    void handleFinished(int idx);

    bool privPreload();

    // Global completion flag protected by a mutex
    static bool playbackCompleted;
    static std::mutex sMtx;        // protects playbackCompleted

    // Startup sequencing flags
    bool preloadsPosted;     // set after we push the initial 20 fills

    unsigned char pad[7];

private:
    // Event-loop gating 
    std::mutex evtMtx;
    std::condition_variable evtCv;
};

#endif

// --- End of File ---
