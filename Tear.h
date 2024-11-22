#pragma once
#include "DynamicObject.h"

namespace agp
{
    class Tear;
}

class agp::Tear : public DynamicObject
{
private:
    RenderableObject* _shadow;
    float _h;  // altezza simulata [ ipotizziamo due altezze nel gioco, h va da 0 a 1, 0.5 divide le due zone ]
    float _absVel;
    float _x_inertia;
    float _y_inertia;

    std::map<std::string, Sprite*> _sprites;
public:
    Tear(Scene* scene, const PointF& pos, Direction dir, float x_inertia, float y_inertia, int layer = 0);
    virtual ~Tear() {};

    void destroy(CollidableObject* obj);

    void update(float dt) override;

    virtual bool collision(CollidableObject* with, Direction fromDir) override;
    virtual bool collidableWith(CollidableObject* obj) override;
    virtual void kill() override;
};
