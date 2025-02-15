#include "Bone.h"
#include "SpriteFactory.h"
#include "Audio.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "GameScene.h"
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
#include "Gusher.h"
#include "Host.h"
#include "Door.h"
using namespace agp;

Bone::Bone(Scene* scene, const PointF& pos, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 1.8f, 1.8f), nullptr, 5)
{
    _sprites["bone_projectile"] = SpriteFactory::instance()->get("bone_projectile");
    _sprites["bone_item"] = SpriteFactory::instance()->get("bone_item");
	_sprite = _sprites["bone_projectile"];
	_item = true;

    _vel = { 0.0f, 0.0f };
    _x_dec_rel = 0;
    _y_dec_rel = 0;
    _dir = { 0.0f, 0.0f };

    _shadow = new RenderableObject(scene, RectF(pos.x + 0.34f, pos.y + 1.3f, 0.37f, 0.23f), _sprites["shadow"], 5);

}

Bone::Bone(Scene* scene, const PointF& pos, Direction dir, float x_velIsaac, float y_velIsaac, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 1.2f, 1.2f), nullptr, layer)
{

    _sprites["bone_projectile"] = SpriteFactory::instance()->get("bone_projectile");

    _item = false;
    _collider.adjust(0.43f, 0.37f, -0.4f, -0.4f);


    //fisica
    pos0 = _rect.pos;
    _absVel = 7.5f;
    _vel = { 0.0f, 0.0f };
    _x_dec_rel = 0;
    _y_dec_rel = 0;
    _x_velIsaac = x_velIsaac * 0.6f;
    _y_velIsaac = y_velIsaac * 0.6f;

    float distanceMax = 8.0f;
    float distanceMin = 5.5f;

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
        _vel = { _x_velIsaac, -_absVel + (_y_velIsaac < 0 ? _y_velIsaac : 0) };
        _distance = _y_velIsaac < 0 ? distanceMax : distanceMin;
        break;
    case Direction::DOWN:
        _vel = { _x_velIsaac, _absVel + (_y_velIsaac > 0 ? _y_velIsaac : 0) };
        _distance = _y_velIsaac > 0 ? distanceMax : distanceMin;
        break;
    default:
        _vel = { 0.0f, 0.0f };
        break;
    }

    //_shadow = new ShadowTear(scene, pos + PointF({ 0.34f,1 }), _vel, 5);
    _shadow = new RenderableObject(scene, RectF(pos.x + 0.34f, pos.y + 1.3f, 0.37f, 0.23f), _sprites["shadow"], 5);

    _sprite = _sprites["bone_projectile"];


}

void Bone::update(float dt)
{
    RenderableObject::update(dt);

    if (_item)
        return;

    if (isSchedule("explosion") || isSchedule("die"))
        return;

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


    float x = _rect.pos.x + _vel.x * dt;
    float y = _rect.pos.y + _vel.y * dt;
    float norm = static_cast<float>(sqrt(pow(pos0.x - x, 2) + pow(pos0.y - y, 2)));
    _rect.pos.x += _vel.x * dt;
	_rect.pos.y += _vel.y * dt;

}


void Bone::destroy(CollidableObject* obj)
{
    if (_item)
    {
        schedule("explosion", 0.01f, [this]() {
            _vel = { 0.0f,0.0f };
            _collidable = false;
            _shadow->setVisible(false);
            setVisible(false);
            }, 0, false);
        schedule("die", 1.3f, [this]() {     
            kill();
            _scene->killObject(_shadow);
            }, 0, false);
    }
    else
    {
        schedule("explosion", 0.01f, [this]() {
            _vel = { 0.0f,0.0f };
            _collidable = false;
            _shadow->setVisible(false);
            setVisible(false);
            Audio::instance()->playSound("bone projectile");
            }, 0, false);
        schedule("die", 1.0f, [this]() {
            kill();
            _scene->killObject(_shadow);
            }, 0, false);
    }
}

bool Bone::collision(CollidableObject* with, Direction fromDir)
{
    if (_item)
    {
        Isaac* isaac = dynamic_cast<Isaac*>(with);
		if (isaac)
		{
            destroy(nullptr);
			isaac->selectItemBone();
			return true;
		}
    }
    else {
        destroy(with);
        Enemy* enemy = dynamic_cast<Enemy*>(with);
        if (enemy)
            enemy->hit(0.5f, _vel);

        Poop* poop = dynamic_cast<Poop*>(with);
        if (poop)
        {
            poop->destroy();
        }
    }
    return true;
}

bool Bone::collidableWith(CollidableObject* obj)
{
    if (_item)
    {
        if (obj->to<Isaac*>())
        {
			return true;
        }
    }
    else
    {
        if (obj->to<Isaac*>() || obj->to<Coin*>())
            return false;
    }


    StaticObject* sobj = obj->to<StaticObject*>();
    if (sobj)
    {

        if (sobj->name().find("up") == 0)
        {
            return false;
        }
    }

    Door* dobj = obj->to<Door*>();
    if (dobj)
    {
        if (dobj->doorPosition() == DoorPosition::TOP)
        {
            return false;
        }
    }

    return true;
}