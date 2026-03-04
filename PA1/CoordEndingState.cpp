#include "CoordEndingState.h"
#include "CoordThread.h"
#include "CoordFinishedState.h"

CoordEndingState& CoordEndingState::Instance()
{
    static CoordEndingState s;
    return s;
}

void CoordEndingState::Update(CoordThread& ctx)
{
    Command* pCmd = nullptr;

    // Only service commands posted by playback; no new file transfers in Ending
    while (ctx.inQueue.PopFront(pCmd))
    {
        pCmd->Execute();
        pCmd = nullptr;
    }

    if (!CoordThread::IsDraining())
    {
        ctx.SetState(&CoordFinishedState::Instance());
    }
}