#pragma once
#include "DynamicObject.h"
#include "CollidableObject.h"
#include "Isaac.h"

namespace agp
{
    class Heart;
}

class agp::Heart : public DynamicObject
{
private:
    float _h;
    float _y;
    std::map<std::string, Sprite*> _sprites;
    bool _colliding;

public:
    Heart(Scene* scene, const PointF& pos, int layer = 0);
    virtual ~Heart() {};

    void destroy(CollidableObject* isaac);

    void update(float dt) override;
    virtual bool collision(CollidableObject* with, Direction fromDir) override;
    virtual bool collidableWith(CollidableObject* with) override;
};