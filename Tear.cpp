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
using namespace agp;

Tear::Tear(Scene* scene, const PointF& pos, Direction dir, int layer)
    : DynamicObject(scene, RectF(pos.x, pos.y, 1, 1), SpriteFactory::instance()->get("tears_default"), layer)
{
    _h = 1;
    _absVel = 8.0f;
    _compenetrable = true;
    _vel = { 0.0f, 0.0f };
    _x_dec_rel = 0;
    _y_dec_rel = 0;
    
	switch (dir) {
	case Direction::LEFT:
        _vel = { -_absVel, 0.0f };
		break;
	case Direction::RIGHT:
        _vel = { _absVel, 0.0f };
		break;
	case Direction::UP:
        _vel = { 0.0f, -_absVel };
		break;
	case Direction::DOWN:
        _vel = { 0.0f, _absVel };
		break;
	default:
        _vel = { 0.0f, 0.0f };
		break;
	}
}

void Tear::update(float dt)
{
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
    schedule("die", 0.001f, [this]() {kill(); }); //schedule evita che crasha il gioco - comunque utile per aspettare che finisca l'animazione di scomparsa
}

bool Tear::collidableWith(CollidableObject* obj)
{
    Isaac* isaac = dynamic_cast<Isaac*>(obj);
    Enemy* enemy = dynamic_cast<Enemy*>(obj);
    if (!isaac)
    {
        if (enemy)    // se incontra un nemico colpiscilo
            destroy(enemy);
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

