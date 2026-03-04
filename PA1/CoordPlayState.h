#ifndef COORDPLAYSTATE_H
#define COORDPLAYSTATE_H

#include "CoordState.h"

class CoordPlayState : public CoordState {
public:
    static CoordPlayState& Instance();
    void Update(CoordThread& ctx) override;
};

#endif