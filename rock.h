#pragma once
#include "StaticObject.h"

namespace agp
{
    class Rock;
}

class agp::Rock : public StaticObject
{
private:

    std::map<std::string, Sprite*> _sprites;

public:
    Rock(Scene* scene, const PointF& pos, int layer = 0);
    virtual ~Rock() {};

    //void destroy();

};

