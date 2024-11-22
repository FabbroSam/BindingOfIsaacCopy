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

Tear::Tear(Scene* scene, const PointF& pos, Direction dir, float x_inertia, float y_inertia, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 1, 1), nullptr, layer)
{
    _sprites["tear"] = SpriteFactory::instance()->get("tears_default");
    _sprites["tears_explosion"] = SpriteFactory::instance()->get("tears_explosion");
    _sprites["shadow"] = SpriteFactory::instance()->get("shadow");
    _sprite = _sprites["tear"];

    _shadow = new RenderableObject(_scene, RectF(0, 0, 0, 0), _sprites["shadow"],4);

    _collider.adjust(0.3f, 0.3f, -0.3f, -0.3f);
    _h = 1;
    _absVel = 8.0f;
    _compenetrable = true;
    _vel = { 0.0f, 0.0f };
    _x_dec_rel = 0;
    _y_dec_rel = 0;
    _x_inertia = x_inertia;
    _y_inertia = y_inertia;



	switch (dir) {
	case Direction::LEFT:
        _vel = { -_absVel+_x_inertia, _y_inertia };
		break;
	case Direction::RIGHT:
        _vel = { _absVel+_x_inertia, _y_inertia };
		break;
	case Direction::UP:
        _vel = { _x_inertia, -_absVel+_y_inertia };
		break;
	case Direction::DOWN:
        _vel = { _x_inertia, _absVel+_y_inertia };
		break;
	default:
        _vel = { 0.0f, 0.0f };
		break;
	}
}

void Tear::update(float dt)
{
    _shadow->setRect(RectF(_rect.pos.x + 0.6f, _rect.pos.y + 0.6f, 0.3, 0.3));

    float gravita = 0.6f; //si può pensare di renderla una variabile nella classe Movable

    if(_h > 0)
        _h = _h - gravita * dt;

    DynamicObject::update(dt);
}

bool Tear::collision(CollidableObject* with, Direction fromDir)
{
    return true;
}

void Tear::destroy(CollidableObject* obj)
{
    obj->setFocused(true);
    _vel = { 0.0f,0.0f };
    _collidable = false;
    // settare la sprite di scomparsa
    schedule("die", 0.02f, [this]() {kill(); }); //schedule evita che crasha il gioco - comunque utile per aspettare che finisca l'animazione di scomparsa
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

        else if (_h > 0.5f) //se h > 0.5 colpisci solo i muri alti, altrimenti tutto il resto
        {
            if (obj->sprite())
                if (!obj->sprite()->name().find("upWall"))
                    destroy(obj);
        }
        else
            destroy(obj);
    }

    return false; // false per non risolvere le collisioni (vogliamo gestire noi le collisioni)
}

void Tear::kill() {
    _sprite = _sprites["tears_explosion"];
    schedule("explosion", 0.3f, [this]() {_scene->killObject(this); });
}