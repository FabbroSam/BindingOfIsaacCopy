#pragma once
#include "DynamicObject.h"
#include "CollidableObject.h"
#include "Isaac.h"

namespace agp
{
    class Coin;
}

class agp::Coin : public DynamicObject
{
private:
    float _h;
    float _y;
    std::map<std::string, Sprite*> _sprites;

public:
    Coin(Scene* scene, const PointF& pos, int layer = 0);
    virtual ~Coin() {};

    void destroy(CollidableObject* isaac);

    void update(float dt) override;
    bool collidableWith(CollidableObject* obj) override;
};