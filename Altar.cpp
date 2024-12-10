#include "Altar.h"
#include "Isaac.h"
#include "DynamicObject.h"
#include <iostream>

using namespace agp;

Altar::Altar(Scene* scene, const RectF& rect, Sprite* sprite, int layer, float angle, SDL_RendererFlip flip)
		: DynamicObject(scene, rect, sprite, layer){
    _originalPosition = _rect.pos;
}

bool Altar::collision(CollidableObject* with, Direction fromDir) {
    if (dynamic_cast<Isaac*>(with)) {
        float deltaX = _rect.pos.x - _originalPosition.x;
        float deltaY = _rect.pos.y - _originalPosition.y;

        const float springConstant = 7.0f;

        float reactionForceX = springConstant * deltaX;
        float reactionForceY = springConstant * deltaY;

        Isaac* isaac = dynamic_cast<Isaac*>(with);
        if (isaac) {
            switch (fromDir) {
            case Direction::LEFT:
                if (_rect.pos.x > _originalPosition.x - 0.3f) {
                    _rect.pos.x -= 0.1f;
                    setVelX(-_pushVel);
                }
                isaac->setVelX(isaac->vel().x - reactionForceX);
                break;

            case Direction::RIGHT:
                if (_rect.pos.x < _originalPosition.x + 0.3f) {
                    _rect.pos.x += 0.1f;
                    setVelX(_pushVel);
                }
                isaac->setVelX(isaac->vel().x - reactionForceX);
                break;

            case Direction::UP:
                if (_rect.pos.y > _originalPosition.y - 0.3f) {
                    _rect.pos.y -= 0.1f;
                    setVelY(-_pushVel);
                }
                isaac->setVelY(isaac->vel().y - reactionForceY);
                break;


            case Direction::DOWN:
                if (_rect.pos.y < _originalPosition.y + 0.3f) {
                    _rect.pos.y += 0.1f;
                    setVelY(_pushVel);
                }
                isaac->setVelY(isaac->vel().y - reactionForceY);
                break;
            }
        }

        schedule("return", 0.1f, [this] {
            const float epsilon = 0.001f;

            if (std::abs(_rect.pos.x - _originalPosition.x) > epsilon) {
                _rect.pos.x += (_originalPosition.x - _rect.pos.x) * 0.5f;
            }
            if (std::abs(_rect.pos.y - _originalPosition.y) > epsilon) {
                _rect.pos.y += (_originalPosition.y - _rect.pos.y) * 0.5f;
            }

            if (std::abs(_rect.pos.x - _originalPosition.x) <= epsilon &&
                std::abs(_rect.pos.y - _originalPosition.y) <= epsilon) {
                _rect.pos = _originalPosition;
                setVelX(0.0f);
                setVelY(0.0f);
            }
            });
    }
    return true;
}
