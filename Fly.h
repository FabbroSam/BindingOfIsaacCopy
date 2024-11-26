#pragma once
#include "Enemy.h"
#include <map>
#include <string>


namespace agp
{
	class Fly;
}

class agp::Fly : public Enemy
{
protected:

	std::map<std::string, Sprite*> _sprites;

public:

	Fly(Scene* scene, const PointF& pos, float _spawnDelay);

	virtual void spawn() {};
	virtual void move();
	virtual void hit(float damage, Vec2Df _dir = { 0,0 });
	virtual void die();

	virtual void update(float dt) override;
	virtual bool collidableWith(CollidableObject* obj) override;
	virtual std::string name() override { return strprintf("Fly[%d]", _id); }
};