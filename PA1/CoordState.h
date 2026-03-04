#ifndef COORDSTATE_H
#define COORDSTATE_H

class CoordThread;

class CoordState {
public:
    virtual ~CoordState() = default;
    virtual void Update(CoordThread& ctx) = 0;
};
#endif