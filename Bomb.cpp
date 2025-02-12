#include <iostream>
#include <cstdlib>
#include "Bomb.h"
#include "Enemy.h"
#include "Isaac.h"
#include "SpriteFactory.h"
#include "Game.h"
#include "GameScene.h"
#include "Audio.h"
#include "AnimatedSprite.h"
#include "Scene.h"
#include "Poop.h"

using namespace agp;

Bomb::Bomb(Scene* scene, const PointF& pos, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 1.0f, 1.0f), nullptr, layer),
    _state(SDL_GetKeyboardState(0))
{
    _collider.adjust(0.9f, 0.9f, -0.7f, -0.7f);
    _scene = scene;
    _sprites["item_bomb"] = SpriteFactory::instance()->get("item_bomb");
    _sprites["item_bomb_explotion"] = SpriteFactory::instance()->get("item_bomb_explotion");
    _sprite = _sprites["item_bomb"];

    _collidable = true;
    _compenetrable = true;
    _h = _rect.pos.y;
    _y = _rect.pos.y;

    _vel.y = -5.0f - static_cast<float>(rand()) / RAND_MAX * 15.0f;
    _vel.x = static_cast<float>(rand()) / RAND_MAX * 3.0f - 1.5f;

    _bombState = BombState::Inactive;
}

void Bomb::update(float dt)
{
    RenderableObject::update(dt);
    Isaac* isaac = static_cast<GameScene*>(_scene)->player();

    switch (_bombState)
    {
    case BombState::Attached:
        _rect.pos.x = isaac->rect().pos.x;
        _rect.pos.y = isaac->rect().pos.y - 0.6f;
        _lastIsaacPos = isaac->rect().pos;

        if (_state[SDL_SCANCODE_E])
        {
            _visible = true;
            Game::instance()->hud()->setBombs(-1);
            _bombState = BombState::Thrown;
        }
        break;

    case BombState::Thrown:
        _vel = { 0,0 };
        _rect.pos = _lastIsaacPos;
        explode();
        break;

    case BombState::Inactive:

        if (_rect.pos.y > _y && _vel.y > 0)
        {
            break;
        }
        else
        {
            _vel.y += 100 * dt;
            _rect.pos.x += _vel.x * dt;
            _rect.pos.y += (50.0f * static_cast<float>(pow(dt, 2))) / 2.0f + _vel.y * dt;
        }
        break;
    case BombState::Exploded:
        break;
    }
}

bool Bomb::collision(CollidableObject* with, Direction fromDir)
{
    Isaac* isaac = dynamic_cast<Isaac*>(with);
    if (isaac && _bombState == BombState::Inactive)
    {
        Audio::instance()->playSound("plop");
        _bombState = BombState::Attached;
        _visible = false;
        Game::instance()->hud()->setBombs(1);
        return true;
    }
    return false;
}

void Bomb::explode()
{
    if (_bombState == BombState::Exploded)
        return;
    else
    {

        schedule("explotion", 1.5f, [this]() {
            _sprite = _sprites["item_bomb_explotion"];
            Audio::instance()->playSound("Grenade pin pull then throw - sound effect-1");
            _rect.size = { 3 , 3 };
            _rect.pos.x -= (_rect.size.x) / 3;
            _rect.pos.y -= (_rect.size.y) / 2;
            RectF _newRect = _rect;
            _newRect.pos.y += 1.0f;
            new RenderableObject(_scene, _newRect, SpriteFactory::instance()->get("bomb_hole"), 7);
           
            for (auto& obj : _scene->objects())
            {
                Vec2Df objPos = obj->rect().pos;
                float dist = _rect.pos.distance(objPos);

                if (dist < 2.5f)
                {
                    if (auto enemy = obj->to<Enemy*>())
                    {
                        enemy->hit(4.0f);
                    }
                    else if (auto poop = obj->to<Poop*>())
                    {
                        poop->destroy();
                        poop->destroy();
                        poop->destroy();
                        poop->destroy();
                    }
                    else if (auto isaac = obj->to<Isaac*>())
                    {
                        isaac->hurt();
                    }
                }
            }
           
            schedule("bomb_kill", 0.34f, [this]() {
                setVisible(false);
                kill();
                }, 0, false);
            });

        _bombState = BombState::Exploded;
    }
}
