#pragma once
#include "DynamicObject.h"
#include "CollidableObject.h"
#include "Isaac.h"

namespace agp
{
    class Bomb;
}

class agp::Bomb : public DynamicObject
{
private:
    float _h;
    float _y;
    const Uint8* _state;
    bool _attachedToIsaac;
    bool _exploded;
    PointF _lastIsaacPos;
    std::map<std::string, Sprite*> _sprites;

public:
    Bomb(Scene* scene, const PointF& pos, int layer = 0);
    void explode();
    virtual ~Bomb() {};

    void update(float dt) override;
    virtual bool collision(CollidableObject* with, Direction fromDir) override;
};