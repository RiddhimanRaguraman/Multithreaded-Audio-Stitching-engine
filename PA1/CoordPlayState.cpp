#include "CoordPlayState.h"
#include "CoordThread.h"
#include "CoordInitState.h"
#include "CoordEndingState.h"
#include "CoordFinishedState.h"
#include "FileThread.h"
#include "UnitTestThread.h"

CoordPlayState& CoordPlayState::Instance()
{
    static CoordPlayState s;
    return s;
}

void CoordPlayState::Update(CoordThread& ctx)
{
    // If a new file arrived, switch to Reading state and wake coordinator
    {
        std::unique_lock<std::mutex> lck_fc(ctx.share_fc.mtx);
        if (ctx.share_fc.status == FileCoordShare::Status::Ready)
        {
            ctx.SetState(&CoordInitState::Instance());
            CoordThread::NotifyCoordWork();
            return;
        }
    }

    // Process all pending WaveFill commands (send coordinator -> wave buffers)
    Command* pCmd = nullptr;
    while (ctx.inQueue.PopFront(pCmd))
    {
        pCmd->Execute();
        pCmd = nullptr;
    }

    const bool producerDone = (FileThread::GetFileReadCount() >= FileThread::MAX_NUM_WAVE_FILES);
    const bool draining     = CoordThread::IsDraining();

    if (producerDone)
    {
        if (draining)
        {
            ctx.SetState(&CoordEndingState::Instance());
        }
        else
        {
            ctx.SetState(&CoordFinishedState::Instance());
        }
    }
}