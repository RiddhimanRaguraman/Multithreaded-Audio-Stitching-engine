#ifndef COORDENDINGSTATE_H
#define COORDENDINGSTATE_H

#include "CoordState.h"

class CoordEndingState : public CoordState {
public:
    static CoordEndingState& Instance();
    void Update(CoordThread& ctx) override;
};

#endif