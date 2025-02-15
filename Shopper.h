#pragma once

#include "StaticObject.h"
#include "Scene.h"
#include "Sprite.h"

namespace agp
{
	class Shopper : public StaticObject
    {
    private:
      
        bool _use;
        std::map<std::string, Sprite*> _sprites;

    public:
        Shopper(Scene* scene, const PointF& pos, int layer = 0);

        void update(float dt);

        virtual bool collision(CollidableObject* with, Direction fromDir) override;
    };
}
