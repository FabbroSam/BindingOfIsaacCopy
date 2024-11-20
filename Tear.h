#pragma once
#include "DynamicObject.h"

namespace agp
{
    class Tear;
}

class agp::Tear : public DynamicObject
{
private:
    float _absVel = 4.0f;

public:
    Tear(Scene* scene, const PointF& pos, Direction dir, int layer = 0);
    void update(float dt) override;
    virtual bool collision(CollidableObject* with, Direction fromDir) override;
};
