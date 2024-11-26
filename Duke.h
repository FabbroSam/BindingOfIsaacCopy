#pragma once

#include "Enemy.h"
#include <map>
#include <string>

namespace agp
{
	class Duke;
}

class agp::Duke : public Enemy
{
protected:

	std::map<std::string, Sprite*> _sprites;
	float _accumulator;
	float _wobbleAccumulator;
	int _heart; //life
	bool _wobbling;
	RenderableObject* _blood;

public:

	Duke(Scene* scene, const PointF& pos, float _spawnDelay);

	//setter of scheduling parameters such as sawn delay, collidable flag ecc...
	void wobble(float dt);

	virtual void spawn() {};
	virtual void move() {};
	virtual void hit(float damage, Vec2Df _dir);
	virtual void die() {};
	virtual bool collidableWith(CollidableObject* obj) override;
	virtual void update(float dt) override;
	virtual bool collision(CollidableObject* with, Direction fromDir) override;
	virtual std::string name() override { return strprintf("Duke[%d]", _id); }
};