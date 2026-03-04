#include "CoordFinishedState.h"
#include "CoordThread.h"

CoordFinishedState& CoordFinishedState::Instance()
{
    static CoordFinishedState s;
    return s;
}

void CoordFinishedState::Update(CoordThread& ctx)
{
    (void)ctx;
    // Finished: no-op. The thread will exit when KillEvent() is set.
}