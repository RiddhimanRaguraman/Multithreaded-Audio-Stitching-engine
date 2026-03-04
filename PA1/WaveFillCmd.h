//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef WAVE_FILL_REQUEST_CMD_H
#define WAVE_FILL_REQUEST_CMD_H

#include "Command.h"
#include "WaveBuffer.h"

class PlayCoordShare;

class WaveFillCmd :public Command
{
public:
	explicit WaveFillCmd(WaveBuffer *p, bool preload);

	WaveFillCmd() = delete;
	WaveFillCmd(const WaveFillCmd &) = delete;
	WaveFillCmd &operator = (const WaveFillCmd &) = delete;
	~WaveFillCmd() = default;

	// Contract for the command
	virtual void Execute() override;

private:
    // 8-byte members first
    WaveBuffer *pWave;
    // small flags last with explicit pad to align class
    bool isPreload;
    unsigned char _pad_align8[7];
};

#endif

// --- End of File ---

