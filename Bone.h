#pragma once
#include "DynamicObject.h"

namespace agp
{
    class Bone;
    class ShadowTear;
}

class agp::Bone : public DynamicObject
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
    Vec2Df _dir;

    bool _item;

    std::map<std::string, Sprite*> _sprites;

public:
    Bone(Scene* scene, const PointF& pos, int layer = 0);
    Bone(Scene* scene, const PointF& pos, Direction dir, float x_velIsaac, float y_velIsaac, int layer = 0);
    virtual ~Bone() {};

    void destroyItem();
    void destroy(CollidableObject* obj);

    void update(float dt) override;

    virtual bool collision(CollidableObject* with, Direction fromDir) override;
    virtual bool collidableWith(CollidableObject* obj) override;
};


