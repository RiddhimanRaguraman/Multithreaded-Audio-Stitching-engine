#ifndef COORDFINISHEDSTATE_H
#define COORDFINISHEDSTATE_H

#include "CoordState.h"

class CoordFinishedState : public CoordState {
public:
    static CoordFinishedState& Instance();
    void Update(CoordThread& ctx) override;
};

#endif