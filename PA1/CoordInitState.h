#ifndef COORDINITSTATE_H
#define COORDINITSTATE_H

#include "CoordState.h"

class CoordInitState : public CoordState {
public:
    static CoordInitState& Instance();
    void Update(CoordThread& ctx) override;
};

#endif