//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "CoordThread.h"
#include "FileThread.h"
#include "Command.h"

#include "UnitTestThread.h"
#include "CoordInitState.h"
#include "CoordPlayState.h"
#include "CoordEndingState.h"
#include "CoordFinishedState.h"

// 4-byte alignment helper for 16-bit stereo PCM frames
static inline size_t align4(size_t n) { return n & ~size_t(3); }

// Singleton access pointer used by static helpers
CoordThread *CoordThread::sInstance = nullptr;

// First-drain barrier statics
std::mutex CoordThread::sFirstDrainMtx;
std::condition_variable CoordThread::sFirstDrainCv;
bool CoordThread::sFirstDrainReady = false;

CoordThread::CoordThread(const char *const pName,
                         FileCoordShare &_share_fc,
                         CircularData &CoordInQueue)
    : KillableBase(pName), 
	mThread(), 
    share_fc(_share_fc), 
    pBuff_A(new Buffer(CoordThread::BUFFER_SIZE, Buffer::Name::CoordA)), 
    pBuff_B(new Buffer(CoordThread::BUFFER_SIZE, Buffer::Name::CoordB)), 
    pFront(pBuff_B),
    pBack(pBuff_A),
    frontPos(0),
    backPos(0),
    inQueue(CoordInQueue), 
    coordHasWork(false)
{
    assert(pBuff_A);
    assert(pBuff_B);

	// Register buffers with Unit Test
	UnitTestThread::CoordA_BufferInfo(this->pBuff_A->GetRawBuff(), this->pBuff_A->GetTotalSize());
    UnitTestThread::CoordB_BufferInfo(this->pBuff_B->GetRawBuff(), this->pBuff_B->GetTotalSize());

    // set singleton
    CoordThread::sInstance = this;

    // initial state
    this->mState = &CoordInitState::Instance();
}

void CoordThread::operator()()
{
    START_BANNER;

    while(!KillEvent())
    {
        bool haveWork = false;
        {
            std::unique_lock<std::mutex> lk(this->evtMtx);
            haveWork = this->evtCv.wait_for(
                lk,
                0ms,
                [&]
                {
                    if (KillEvent()) return true;
                    {
                        std::unique_lock<std::mutex> lck_fc(share_fc.mtx);
                        if (share_fc.status == FileCoordShare::Status::Ready)
                            return true;
                    }
                    if (this->coordHasWork)
                        return true;
                    return false;
                });
            if (haveWork)
            {
                this->coordHasWork = false;
            }
        }

        if (!haveWork)
        {
            continue;
        }

        if (this->mState)
        {
            this->mState->Update(*this);
        }
    }

}

CoordThread::~CoordThread()
{
    if(this->mThread.joinable())
    {
        this->mThread.join();
    }

    // Flush any leftover commands in coordinator input queue on teardown
    Command *cmd = nullptr;
    while (inQueue.PopFront(cmd))
    {
        delete cmd; // discard on teardown
        cmd = nullptr;
    }

    delete pBuff_A;
    pBuff_A = nullptr;

    delete pBuff_B;
	pBuff_B = nullptr;

}

void CoordThread::Launch()
{
	if(this->mThread.joinable() == false)
	{
		this->mThread = std::thread(std::ref(*this));
	}
	else
	{
		assert(false);
	}
}


void CoordThread::privSwapBuffers()
{
    // Precondition: coordMtx is held by the caller
    assert((this->frontPos % 4) == 0);
    assert((this->backPos  % 4) == 0);
    assert(this->frontPos == this->pFront->GetCurrSize() && "Swap attempted before front fully drained");

    std::swap(this->pFront, this->pBack);
    std::swap(this->frontPos,  this->backPos);
    this->backPos = 0;
    this->pBack->SetCurrSize(0);

    Debug::out("\t  Buffer Swap: Front=%s, Back=%s \n",
        (pFront == pBuff_A) ? "A" : "B",
        (pBack  == pBuff_A) ? "A" : "B");

}

// Copy up to 2048 bytes (2KB), frame-aligned; top-off from back; swap on front drain; zero-pad end.
size_t CoordThread::CopyNext2KTo(unsigned char *pDest, size_t destSize)
{
    CoordThread* c = CoordThread::Instance();
    if (!c || !pDest || destSize < 2048) return 0;
    size_t want = 2048; size_t n1; size_t n2;
    std::unique_lock<std::mutex> lk(c->coordMtx);

    n1 = align4(std::min(want, (c->pFront->GetCurrSize() > c->frontPos)
        ? (c->pFront->GetCurrSize() - c->frontPos) : size_t(0)));
    if (n1) {
        std::memcpy(pDest, c->pFront->GetRawBuff() + c->frontPos, n1);
        UnitTestThread::WaveDataTransfer(pDest, n1, c->pFront->GetRawBuff() + c->frontPos, n1);
        c->frontPos += n1; want -= n1; pDest += n1;
    }

    n2 = align4(std::min(want, (c->pBack->GetCurrSize() > c->backPos)
        ? (c->pBack->GetCurrSize() - c->backPos) : size_t(0)));
    if (n2) {
        std::memcpy(pDest, c->pBack->GetRawBuff() + c->backPos, n2);
        UnitTestThread::WaveDataTransfer(pDest, n2, c->pBack->GetRawBuff() + c->backPos, n2);
        c->backPos += n2; want -= n2; pDest += n2;
    }

    if (want > 0) { std::memset(pDest, 0, want); }

    if (c->frontPos >= c->pFront->GetCurrSize() && c->pBack->GetCurrSize() > c->backPos)
        c->privSwapBuffers();

    return 2048 - want;
}


size_t CoordThread::DrainRemaining()
{
    CoordThread* p = CoordThread::Instance();
    if (!p) return 0;
    std::lock_guard<std::mutex> lk(p->coordMtx);
    size_t frontLeft = (p->pFront->GetCurrSize() > p->frontPos) ? (p->pFront->GetCurrSize() - p->frontPos) : 0;
    size_t backLeft  = (p->pBack->GetCurrSize()  > p->backPos ) ? (p->pBack->GetCurrSize()  - p->backPos ) : 0;
    return frontLeft + backLeft;
}

bool CoordThread::IsDraining()
{
    CoordThread* p = CoordThread::Instance();
    if (!p) return false;
    std::lock_guard<std::mutex> lk(p->coordMtx);
    size_t frontLeft = (p->pFront->GetCurrSize() > p->frontPos) ? (p->pFront->GetCurrSize() - p->frontPos) : 0;
    size_t backLeft  = (p->pBack->GetCurrSize()  > p->backPos ) ? (p->pBack->GetCurrSize()  - p->backPos ) : 0;
    return (frontLeft + backLeft) > 0;
}

// One-shot notify to signal the first time a drainable buffer exists
void CoordThread::SignalFirstDrain()
{
    std::lock_guard<std::mutex> lk(CoordThread::sFirstDrainMtx);
    if (!CoordThread::sFirstDrainReady)
    {
        CoordThread::sFirstDrainReady = true;
        CoordThread::sFirstDrainCv.notify_all();
    }
}

// Block until the first drainable buffer is prepared
void CoordThread::WaitForFirstDrain()
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lk(CoordThread::sFirstDrainMtx);
            if (CoordThread::sFirstDrainReady)
                break;
        }
    }
}

void CoordThread::NotifyCoordWork()
{
    CoordThread* inst = CoordThread::Instance();
    if (!inst) return;
    {
        std::lock_guard<std::mutex> lk(inst->evtMtx);
        inst->coordHasWork = true;
    }
    inst->evtCv.notify_one();
}
// --- End of File ---
CoordThread* CoordThread::Instance()
{
    return CoordThread::sInstance;
}
