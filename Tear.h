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

    int _cases;
    Vec2Df _x0y0;
    Vec2Df _xy;

    std::map<std::string, Sprite*> _sprites;

public:
    Tear(Scene* scene, const PointF& pos, Direction dir, float x_inertia, float y_inertia, int layer = 0);
    virtual ~Tear() {};

    void destroy(CollidableObject* obj);

    void update(float dt) override;

    virtual bool collision(CollidableObject* with, Direction fromDir) override;
    virtual bool collidableWith(CollidableObject* obj) override;
};

class agp::ShadowTear : public DynamicObject
{
private:

    std::map<std::string, Sprite*> _sprites;

public:
    ShadowTear(Scene* scene, const PointF& pos, const Vec2Df& vel, int layer = 0);
    virtual ~ShadowTear() {};

    void update(float dt) override;
};

