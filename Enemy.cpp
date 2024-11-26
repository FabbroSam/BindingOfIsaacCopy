#include "Enemy.h"
#include "Isaac.h"
#include "Audio.h"
#include "Scene.h"
#include "Tear.h"
#include "StaticObject.h"

using namespace agp;

Enemy::Enemy(Scene* scene, const RectF& rect, Sprite* sprite, float spawnDelay, int layer)
	: DynamicObject(scene, rect, sprite, layer)
{
	_hitable = true;
	_dying = false;
	_spawnDelay = spawnDelay;
}

bool Enemy::collision(CollidableObject* with, Direction fromDir)
{
	// gestione delle collisioni, oggetto per oggetto

	Isaac* isaac = with->to<Isaac*>();
	Tear * tear = with->to<Tear*>();
	if (isaac)
	{
		std::cout << "enemy: isaac collision" << std::endl;
		if (_hitable && isaac->invincible())
			hit();
		else
			isaac->hurt();	
		return true;
	}
	if (tear)
	{
		if (_hitable)
			hit();
	}


	return false;
}

bool Enemy::collidableWith(CollidableObject* obj)
{
	// usare questo metodo per impedire collisioni con alcuni oggetti
	//	if (obj->to<Tear*>())
	//		return false;


	// default: collisione possibile con tutti gli oggetti
	return true;
}