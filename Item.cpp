#include "Item.h"

using namespace agp;

Item::Item(Scene* scene, const RectF& rect, Sprite* sprite, int layer, float angle, SDL_RendererFlip flip)
    : CollidableObject(scene, rect, sprite, layer, angle, flip)
{
    _compenetrable = true;
    _animTimer = -0.3f;
    _phase = 0;
}

void Item::update(float dt)
{
    CollidableObject::update(dt);
    const float phaseDuration = 0.3f;
    _animTimer += dt;
    if (_animTimer >= phaseDuration) {
        _animTimer = -0.3f;
        _phase = (_phase + 1) % 2;
    }

    Vec2D<float> center = _rect.center();
    float baseY = center.y;

    if (_phase == 0) {
        float scaleFactorX = 1.0f - _animTimer / phaseDuration * 0.5f;
        float scaleFactorY = 1.0f + _animTimer / phaseDuration * 0.5f;
        _rect.adjust(0, 0, 0.003f * scaleFactorX, -0.003f * scaleFactorY);
    }
    else {
        float scaleFactorX = 1.0f - _animTimer / phaseDuration * 0.5f;
        float scaleFactorY = 1.0f + _animTimer / phaseDuration * 0.5f;
        _rect.adjust(0, 0, -0.003f * scaleFactorX, 0.003f * scaleFactorY);
    }

    _rect.pos.x = center.x - _rect.size.x / 2;
    _rect.pos.y = baseY + 0.08f * sinf(0.7 * _animTimer) - _rect.size.y / 2;
}