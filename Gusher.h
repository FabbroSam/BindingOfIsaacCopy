#pragma once

#include "Enemy.h"
#include <map>
#include <string>

namespace agp
{
	class Gusher;
}

class agp::Gusher : public Enemy
{
protected:

	std::map<std::string, Sprite*> _sprites;

	float _accumulator;
	bool _blood;
	RenderableObject* _blood_walk;
	Direction _x_prev_dir;
	Direction _y_prev_dir;

public:

	Gusher(Scene* scene, const PointF& pos, float _spawnDelay);

	//basic enemy actions
	virtual void spawn() {};
	virtual void move();
	virtual void hit(float damage, Vec2Df _dir) {};
	virtual void die() {};

	virtual void update(float dt) override;

	void shoot();
	void wobble(float dt) {};
	virtual void trigger() override {};

	virtual bool collision(CollidableObject* with, Direction fromDir) override;
	virtual bool collidableWith(CollidableObject* obj) override;
	virtual std::string name() override { return strprintf("Gusher[%d]", _id); }
};