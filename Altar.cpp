//#include "Altar.h"
//#include "Isaac.h"
//#include "DynamicObject.h"
//#include <iostream>
//
//using namespace agp;
//
//Altar::Altar(Scene* scene, const RectF& rect, Sprite* sprite, int layer, float angle, SDL_RendererFlip flip)
//		: DynamicObject(scene, rect, sprite, layer){
//    _originalPosition = _rect.pos;
//}
//
//bool Altar::collision(CollidableObject* with, Direction fromDir) {
//    Isaac* isaac = dynamic_cast<Isaac*>(with);
//
//    if (isaac) {
//        switch (fromDir) {
//        case Direction::LEFT:
//            _radialDistance = _rect.pos.x - _originalPosition.x;
//            if (std::abs(_radialDistance) < 0.3f) {
//                move_x(Direction::RIGHT);
//                setVelX(_pushVel);
//            }
//            else {
//                setVelX(0.0f);
//                _rect.pos.x = _originalPosition.x;
//            }
//
//            schedule("return", 0.05f, [this, isaac] {
//                move_x(Direction::LEFT);
//                setVelX(-_returnVel);
//                isaac->move_x(Direction::LEFT);
//                isaac->setVelX(-_reactionForce);
//
//                schedule("stop", 0.02f, [this] {
//                    move_x(Direction::NONE);
//                    setVelX(0.0f);
//                    _rect.pos.x = _originalPosition.x;
//                    });
//                });
//            break;
//
//        case Direction::RIGHT:
//            _radialDistance = _rect.pos.x - _originalPosition.x;
//            if (std::abs(_radialDistance) < 0.3f) {
//                move_x(Direction::LEFT);
//                setVelX(-_pushVel);
//            }
//            else {
//                setVelX(0.0f);
//                _rect.pos.x = _originalPosition.x;
//            }
//
//            schedule("return", 0.05f, [this, isaac] {
//                move_x(Direction::RIGHT);
//                setVelX(_returnVel);
//                isaac->move_x(Direction::RIGHT);
//                isaac->setVelX(_reactionForce);
//
//                schedule("stop", 0.02f, [this] {
//                    move_x(Direction::NONE);
//                    setVelX(0.0f);
//                    _rect.pos.x = _originalPosition.x;
//                    });
//                });
//            break;
//
//        case Direction::UP:
//            _radialDistance = _rect.pos.y - _originalPosition.y;
//            if (std::abs(_radialDistance) < 0.3f) {
//                move_y(Direction::DOWN);
//                setVelY(_pushVel);
//            }
//            else {
//                setVelY(0.0f);
//                _rect.pos.y = _originalPosition.y;
//            }
//
//            schedule("return", 0.05f, [this, isaac] {
//                move_y(Direction::UP);
//                setVelY(-_returnVel);
//                isaac->move_y(Direction::UP);
//                isaac->setVelY(-_reactionForce);
//
//                schedule("stop", 0.02f, [this] {
//                    move_y(Direction::NONE);
//                    setVelY(0.0f);
//                    _rect.pos.y = _originalPosition.y;
//                    });
//                });
//            break;
//
//        case Direction::DOWN:
//            _radialDistance = _rect.pos.y - _originalPosition.y;
//            if (std::abs(_radialDistance) < 0.3f) {
//                move_y(Direction::UP);
//                setVelY(-_pushVel);
//            }
//            else {
//                setVelY(0.0f);
//                _rect.pos.y = _originalPosition.y;
//            }
//
//            schedule("return", 0.05f, [this, isaac] {
//                move_y(Direction::DOWN);
//                setVelY(_returnVel);
//                isaac->move_y(Direction::DOWN);
//                isaac->setVelY(-_reactionForce);
//
//                schedule("stop", 0.02f, [this] {
//                    move_y(Direction::NONE);
//                    setVelY(0.0f);
//                    _rect.pos.y = _originalPosition.y;
//                    });
//                });
//            break;
//        }
//    }
//    return true;
//}
//
