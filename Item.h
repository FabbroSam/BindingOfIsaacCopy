#pragma once

#include "CollidableObject.h"
#include "Scene.h"
#include "Sprite.h"

namespace agp
{
    class Item : public CollidableObject
    {
    private:
        float _animTimer;
        int _phase;

    public:
        Item(Scene* scene, const RectF& rect, Sprite* sprite, int layer = 0, float angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

        void update(float dt);
    };
}
