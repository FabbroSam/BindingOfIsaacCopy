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
#include "Duke.h"
#include "Poop.h"
#include "Coin.h"
#include "Rock.h"
using namespace agp;


Tear::Tear(Scene* scene, const PointF& pos, Direction dir, float x_velIsaac, float y_velIsaac, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 1.2f, 1.2f), nullptr, 6)
{

    _sprites["tear"] = SpriteFactory::instance()->get("tear_default");
    _sprites["tear_explosion"] = SpriteFactory::instance()->get("tear_explosion");
    _sprites["tear_wet"] = SpriteFactory::instance()->get("tear_wet");
    _sprites["shadow"] = SpriteFactory::instance()->get("shadow");
    _sprite = _sprites["tear"];

    _destroy = true;

    _collider.adjust(0.43f, 0.37f, -0.4f, -0.4f);

    //fisica
    pos0 = _rect.pos;
    _absVel = 7.0f;
    _vel = { 0.0f, 0.0f };
    _x_dec_rel = 0;
    _y_dec_rel = 0;
    _x_velIsaac = x_velIsaac * 0.6f;
    _y_velIsaac = y_velIsaac * 0.6f;

    float distanceMax = 8.5f;
    float distanceMin = 5.0f;

    switch (dir) {
    case Direction::LEFT:
        _vel = { -_absVel + (_x_velIsaac < 0 ? _x_velIsaac : 0), _y_velIsaac };
        _distance = _x_velIsaac < 0 ? distanceMax : distanceMin;
        break;
    case Direction::RIGHT:
        _vel = { _absVel + (_x_velIsaac > 0 ? _x_velIsaac : 0), _y_velIsaac };
        _distance = _x_velIsaac > 0 ? distanceMax : distanceMin;
        break;
    case Direction::UP:
        _vel = { _x_velIsaac, -_absVel + (_x_velIsaac < 0 ? _x_velIsaac : 0) };
        _distance = _y_velIsaac < 0 ? distanceMax : distanceMin;
        break;
    case Direction::DOWN:
        _vel = { _x_velIsaac, _absVel + (_x_velIsaac > 0 ? _x_velIsaac : 0) };
        _distance = _y_velIsaac > 0 ? distanceMax : distanceMin;
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

    if (isSchedule("explosion") || isSchedule("die"))
        return;

    //resolveCollisions(dt);

    //control tear level respect shadow
    if (_rect.pos.y > _shadow->rect().pos.y - 0.5f)
    {
        destroy(nullptr);
        return;
    }

    // move shadow
    PointF _shadowPos = _shadow->rect().pos + _vel * dt;
    PointF _shadowSize = _shadow->rect().size;
    _shadow->setRect(RectF(_shadowPos.x, _shadowPos.y, _shadowSize.x, _shadowSize.y));

    //move tear
    float x = _rect.pos.x + _vel.x * dt;
    float y = _rect.pos.y + _vel.y * dt;
    float norm = static_cast<float>(sqrt(pow(pos0.x - x, 2) + pow(pos0.y - y, 2)));
    _rect.pos.x += _vel.x * dt;
    if (norm < _distance)
        _rect.pos.y += static_cast<float>((10 * pow(dt,2)) / 2 + _vel.y * dt);
    else
        _rect.pos.y += static_cast<float>((500 * pow(dt, 2)) / 2 + _vel.y * dt);
 
}


void Tear::destroy(CollidableObject* obj)
{
    //if(obj)
    //    obj->setFocused(true);
    
    float newX = _rect.pos.x + (static_cast<float>(rand() % 15000 - 5000) / 9000.0f);
    float newY = _rect.pos.y + (static_cast<float>(rand() % 15000 - 5000) / 9000.0f);
    new RenderableObject(_scene, RectF(newX, newY, (rand() % 20 + 10) / 100.0f, (rand() % 20 + 10) / 100.0f), _sprites["tear_wet"], 2);
    new RenderableObject(_scene, RectF(newX, newY, (rand() % 20 + 10) / 100.0f, (rand() % 20 + 10) / 100.0f), _sprites["tear_wet"], 2);
    new RenderableObject(_scene, RectF(newX, newY, (rand() % 20 + 10) / 100.0f, (rand() % 20 + 10) / 100.0f), _sprites["tear_wet"], 2);

    schedule("explosion", 0.01f, [this]() {
        _sprite = _sprites["tear_explosion"];
        _rect.size = {1.8f, 1.8f};
        _rect.pos += {-0.4f, -0.4f};
        _vel = { 0.0f,0.0f };
        _collidable = false;
        _shadow->setVisible(false);
        Audio::instance()->playSound("tear block");
        }, 0, false);
    schedule("die", 0.4f, [this]() {
        kill();
        _scene->killObject(_shadow);
        },0,false);
}

bool Tear::collision(CollidableObject* with, Direction fromDir)
{
    destroy(with);
    Enemy* enemy = dynamic_cast<Enemy*>(with);
    if (enemy) // se incontra un nemico colpiscilo
    {
        enemy->hit(0.5f, _vel);
    }

    Poop* poop = dynamic_cast<Poop*>(with);
    if (poop)
    {
        poop->destroy();
    }
    return true;
}

bool Tear::collidableWith(CollidableObject* obj)
{
    if (obj->to<Isaac*>() || obj->to<Coin*>())
        return false;
    return true;
}