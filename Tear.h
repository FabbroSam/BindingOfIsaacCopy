#pragma once
#include "DynamicObject.h"

namespace agp
{
    class Tear;
    class ShadowTear;
}

class agp::Tear : public DynamicObject
{
private:
    //ShadowTear* _shadow;
    RenderableObject* _shadow;
    float _distance;
    Vec2Df pos0;
    bool _destroy;
    float _absVel;  
    float _x_velIsaac;
    float _y_velIsaac;
    bool _red;

    std::map<std::string, Sprite*> _sprites;

public:
    Tear(Scene* scene, const PointF& pos, Direction dir, float x_inertia, float y_inertia, bool red, int layer = 0);
    virtual ~Tear() {};

    void destroy(CollidableObject* obj);

    void update(float dt) override;

    virtual bool collision(CollidableObject* with, Direction fromDir) override;
    virtual bool collidableWith(CollidableObject* obj) override;
};


