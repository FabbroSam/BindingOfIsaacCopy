#pragma once
#include "StaticObject.h"
#include "DynamicObject.h"

namespace agp
{
    class Poop;
    class PoopGibs;
}

class agp::Poop : public StaticObject
{
private:
    RenderableObject* _explosion;
    std::string _type;
    int _index;
    std::map<std::string, Sprite*> _sprites;

public:
    Poop(Scene* scene, const PointF& pos, int layer = 0);
    virtual ~Poop() {};

    void destroy();

};

class agp::PoopGibs : public DynamicObject
{
private:
    float _h;
    float _y;

    std::map<std::string, Sprite*> _sprites;

public:
    PoopGibs(Scene* scene, const PointF& pos, std::string type, int layer = 0);
    virtual ~PoopGibs() {};

    void update(float dt) override;

};


