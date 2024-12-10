#include "Item.h"

using namespace agp;

Item::Item(Scene* scene, const RectF& rect, Sprite* sprite, int layer, float angle, SDL_RendererFlip flip, Altar* altar)
    : CollidableObject(scene, rect, sprite, layer, angle, flip), _altar(altar)
{
    _compenetrable = true;
    _animTimer = -0.3f;
    _phase = 0;
}

void Item::update(float dt)
{
    CollidableObject::update(dt);

    _animTimer += dt;

    Altar* altar = getAltar();
    if (!altar) return;

    _rect.pos.x = altar->rect().pos.x;

    float oscillationNormalizedTime = fmod(5 * _animTimer, 2 * M_PI);
    float oscillation = 0.1f * sinf(oscillationNormalizedTime);

    _rect.pos.y = altar->rect().pos.y - 1.0f + oscillation;

    float wobblingNormalizedTime = fmod(10 * _animTimer, 2 * M_PI);
    float xWobbling = 0.01f * sinf(wobblingNormalizedTime);
    float yWobbling = 0.01f * cosf(wobblingNormalizedTime);
    _rect.adjust(0, 0, xWobbling, -yWobbling);
}

void Item::resolveCollisions() {
    ;
}

Altar* Item::getAltar() {
    return _altar;
}