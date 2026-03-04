#include "CoordInitState.h"
#include "CoordThread.h"
#include "CoordPlayState.h"
#include "FileThread.h"
#include "UnitTestThread.h"

static inline size_t align4_local(size_t n) { return n & ~size_t(3); }

CoordInitState& CoordInitState::Instance()
{
    static CoordInitState s;
    return s;
}

void CoordInitState::Update(CoordThread& ctx)
{
    {
        std::unique_lock<std::mutex> lck_fc(ctx.share_fc.mtx);
        if (ctx.share_fc.status == FileCoordShare::Status::Ready)
        {
            ctx.share_fc.status = FileCoordShare::Status::Transfer;
            lck_fc.unlock();

            FileThread::CopyBufferToDest(ctx.pBack);
            {
                std::lock_guard<std::mutex> lk(ctx.coordMtx);
                const size_t sz = align4_local(ctx.pBack->GetCurrSize());
                ctx.pBack->SetCurrSize(sz);
                ctx.backPos = (ctx.backPos > sz) ? sz : ctx.backPos;
                Debug::out("\tFile->Coord transfer: Back=%s size=%zu\n",
                    (ctx.pBack == ctx.pBuff_A) ? "A" : "B", sz);
            }

            UnitTestThread::DataTransfer(ctx.pBack->GetRawBuff(), ctx.pBack->GetCurrSize());

            CoordThread::SignalFirstDrain();

            {
                std::lock_guard<std::mutex> lk2(ctx.coordMtx);
                if (ctx.pFront->GetCurrSize() == ctx.frontPos &&
                    ctx.pBack->GetCurrSize()  > ctx.backPos)
                {
                    ctx.privSwapBuffers();
                }
            }

            std::lock_guard<std::mutex> _lck_fc2(ctx.share_fc.mtx);
            ctx.share_fc.status = FileCoordShare::Status::Empty;
        }
    }

    if (CoordThread::IsDraining())
    {
        ctx.SetState(&CoordPlayState::Instance());
        // Immediately schedule coordinator to process fills
        CoordThread::NotifyCoordWork();
    }
}