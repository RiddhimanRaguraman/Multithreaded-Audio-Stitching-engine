//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef WAVE_FINISHED_CMD_H
#define WAVE_FINISHED_CMD_H

#include "Command.h"

class WaveFinishedCmd : public Command
{
public:
    explicit WaveFinishedCmd(int idx);

    WaveFinishedCmd() = delete;
    WaveFinishedCmd(const WaveFinishedCmd &) = delete;
    WaveFinishedCmd &operator=(const WaveFinishedCmd &) = delete;
    ~WaveFinishedCmd() = default;

    virtual void Execute() override;

    int GetIndex() const { return index; }

private:
    int index;
    int _pad_align8; // explicit padding to satisfy 8-byte alignment and silence C4820
};

#endif

// --- End of File ---