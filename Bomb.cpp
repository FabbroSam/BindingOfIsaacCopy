#include <list>
#include <iostream>
#include "Bomb.h"
#include "Isaac.h"
#include "SpriteFactory.h"
#include "Game.h"
#include "GameScene.h"
#include "Audio.h"
#include "AnimatedSprite.h"
#include "Scene.h"
#include <iostream>
#include <cstdlib>

using namespace agp;

Bomb::Bomb(Scene* scene, const PointF& pos, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 1.0f, 1.0f), nullptr, 9)
{
    _scene = scene;
    _sprites["item_bomb"] = SpriteFactory::instance()->get("item_bomb");
    _sprites["item_bomb_explotion"] = SpriteFactory::instance()->get("item_bomb_explotion");
    _sprite = _sprites["item_bomb"];

    _state = SDL_GetKeyboardState(0);
    _exploded = false;

    _collidable = true;
    _compenetrable = true;

    const double PI = 3.1415926535897932384650288;
    _attachedToIsaac = false;
    _h = _rect.pos.y;
    _x_dec_rel = 0;
    _x_dec_rel = 0;
    _vel.y = -5.0f - static_cast<float>(rand()) / RAND_MAX * 15.0f;
    _vel.x = static_cast<float>(rand()) / RAND_MAX * 3.0f - 1.5f;
    float alpha = -60;
    float d = static_cast<float>(rand()) / RAND_MAX * 1.5f;

    //std::cout << "_vel.x: " << _vel.x << " alpha: " << alpha << " d " << d << std::endl;
    float u_x = static_cast<float>(cos(alpha * PI / 360));
    float u_y = -static_cast<float>(sin(alpha * PI / 360));
    float x = _rect.pos.x + d * u_x;
    _y = _rect.pos.y + d * u_y;

}

void Bomb::update(float dt)
{
    RenderableObject::update(dt);

    Isaac* isaac = static_cast<GameScene*>(_scene)->player();

    if (_attachedToIsaac)
    {
        //teletrasporto?
        _rect.pos.x = isaac->rect().pos.x;
        _rect.pos.y = isaac->rect().pos.y - 0.5f;
    }
    else
    {
        if (_rect.pos.y > _y && _vel.y > 0)
        {
            return;
        }
        else
        {
            _vel.y += 100 * dt;
            _rect.pos.x += _vel.x * dt;
            _rect.pos.y += (60.0f * static_cast<float>(pow(dt, 2))) / 2.0f + _vel.y * dt;
        }
    }

    // se premo E, la bomba acquisisce l'ultima posizione di isaac e poi esplode

    if (!_exploded)
    {
        _lastIsaacPos = isaac->rect().pos;
        
    }
    if (_state[SDL_SCANCODE_E] && !_exploded)
    {
        _rect.pos = _lastIsaacPos;
        explode();
    }

}

    bool Bomb::collision(CollidableObject* with, Direction fromDir)
    {
        Isaac* isaac = dynamic_cast<Isaac*>(with);
        if (isaac && _collidable && !_attachedToIsaac && !_exploded)
        {
            std::cout << "collision() chiamato di nuovo!\n";
            isaac->setBombCarry(true);
            std::cout << "bomba toccata\n";
            _attachedToIsaac = true;

            schedule("invisible", 3.0f, [this, isaac]() {
                std::cout << "Timer scaduto: bomba invisibile e Isaac lascia la bomba\n";
                _visible = false;
                isaac->setBombCarry(false);
                });

            Game::instance()->hud()->setBombs(1);
            // Audio::instance()->playSound("key drop 2");
            return true;
        }
        return false;
    }


    void Bomb::explode()
    {
        _visible = true;
        _exploded = true;
        _attachedToIsaac = false;
    }

