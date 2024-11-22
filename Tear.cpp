#include "Tear.h"
#include "SpriteFactory.h"
#include "Audio.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "Scene.h"
#include <list>
#include <iostream>
#include "Isaac.h"
#include "Enemy.h"
#include "Fly.h"
using namespace agp;


Tear::Tear(Scene* scene, const PointF& pos, Direction dir, float x_velIsaac, float y_velIsaac, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 1.2f, 1.2f), nullptr, 6)
{

    _sprites["tear"] = SpriteFactory::instance()->get("tears_default");
    _sprites["tears_explosion"] = SpriteFactory::instance()->get("tears_explosion");
    _sprites["shadow"] = SpriteFactory::instance()->get("shadow");
    _sprite = _sprites["tear"];

    _destroy = true;

    _compenetrable = true;
    _collider.adjust(0.3f, 0.3f, -0.3f, -0.3f);

    //fisica
    pos0 = _rect.pos;
    _absVel = 7.0f;
    _vel = { 0.0f, 0.0f };
    _x_dec_rel = 0;
    _y_dec_rel = 0;
    _x_velIsaac = x_velIsaac * 0.6f;
    _y_velIsaac = y_velIsaac * 0.6f;

    switch (dir) {
    case Direction::LEFT:
        _vel = { -_absVel + (_x_velIsaac < 0 ? _x_velIsaac : 0), _y_velIsaac };
        _distance = _x_velIsaac < 0 ? 8.5f : 6;
        break;
    case Direction::RIGHT:
        _vel = { _absVel + (_x_velIsaac > 0 ? _x_velIsaac : 0), _y_velIsaac };
        _distance = _x_velIsaac > 0 ? 8.5f : 6;
        break;
    case Direction::UP:
        _vel = { _x_velIsaac, -_absVel + (_x_velIsaac < 0 ? _x_velIsaac : 0) };
        _distance = _y_velIsaac < 0 ? 8.5f : 6;
        break;
    case Direction::DOWN:
        _vel = { _x_velIsaac, _absVel + (_x_velIsaac > 0 ? _x_velIsaac : 0) };
        _distance = _y_velIsaac > 0 ? 8.5f : 6;
        break;
    default:
        _vel = { 0.0f, 0.0f };
        break;
    }

    //_shadow = new ShadowTear(scene, pos + PointF({ 0.34f,1 }), _vel, 5);
    _shadow = new RenderableObject(scene, RectF(pos.x + 0.34f, pos.y + 1.3f, 0.37f, 0.23f), _sprites["shadow"], 5);
}

void Tear::update(float dt)
{
    RenderableObject::update(dt);

    resolveCollisions(dt);

    if (!_destroy)
        return;

    //control tear level respect shadow
    if (_rect.pos.y > _shadow->rect().pos.y - 0.5f)
    {
        destroy(nullptr);
        _destroy = false;
        return;
    }

    // move shadow
    PointF _shadowPos = _shadow->rect().pos + _vel * dt;
    PointF _shadowSize = _shadow->rect().size;
    _shadow->setRect(RectF(_shadowPos.x, _shadowPos.y, _shadowSize.x, _shadowSize.y));

    //move tear
    float x = _rect.pos.x + _vel.x * dt;
    float y = _rect.pos.y + _vel.y * dt;
    float norm = sqrt(pow(pos0.x - x, 2) + pow(pos0.y - y, 2));
    _rect.pos.x += _vel.x * dt;
    std::cout << norm << std::endl;
    if (norm < _distance)
        _rect.pos.y += (10 * pow(dt,2)) / 2 + _vel.y * dt;
    else
        _rect.pos.y += (500 * pow(dt, 2)) / 2 + _vel.y * dt;
 
}


void Tear::destroy(CollidableObject* obj)
{
    if(obj)
        obj->setFocused(true);
    
    if (isSchedule("explosion") || isSchedule("die"))
        return;

    schedule("explosion", 0.01f, [this]() {
        _sprite = _sprites["tears_explosion"];
        _rect.size = {1.8f, 1.8f};
        _rect.pos += {-0.4f, -0.4f};
        _vel = { 0.0f,0.0f };
        _collidable = false;
        _shadow->setVisible(false);
        }, 0, false);
    schedule("die", 0.4f, [this]() {
        kill();
        _scene->killObject(_shadow);
        },0,false);
}

bool Tear::collision(CollidableObject* with, Direction fromDir)
{
    return true;
}

bool Tear::collidableWith(CollidableObject* obj)
{
    Isaac* isaac = dynamic_cast<Isaac*>(obj);
    Enemy* enemy = dynamic_cast<Enemy*>(obj);
    Fly* fly = dynamic_cast<Fly*>(obj);
    if (!isaac)
    {
        if (enemy) // se incontra un nemico colpiscilo
        {  
            destroy(enemy);
            fly->hurt();
        }

        if (obj->sprite())
        {
            std::cout << obj->sprite()->name() << std::endl;
            if (!obj->sprite()->name().find("upWall"))
                destroy(obj);
        }
    }

    return false; // false per non risolvere le collisioni (vogliamo gestire noi le collisioni)
}