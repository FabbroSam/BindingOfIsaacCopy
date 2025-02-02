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
    enum class BombState {
        Inactive,
        Attached,
        Thrown,
        Exploded
    };
    BombState _bombState;
    float _h;
    float _y;
    const Uint8* _state;
    PointF _lastIsaacPos;
    std::map<std::string, Sprite*> _sprites;

public:
    Bomb(Scene* scene, const PointF& pos, int layer = 0);
    void explode();
    void damage(Scene& _scene);
    virtual ~Bomb() {};

    void update(float dt) override;
    virtual bool collision(CollidableObject* with, Direction fromDir) override;
};