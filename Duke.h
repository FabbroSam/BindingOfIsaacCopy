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
	bool _wobbling;
	int _n_fly;
	int _max_flies;
	float index[5]{ 0 };
	float _acc;
	float _fix_pos_x;
	RenderableObject* _blood;
	RenderableObject* _blackglow;
	Direction _x_prev_dir;
	Direction _y_prev_dir;

public:

	Duke(Scene* scene, const PointF& pos, float _spawnDelay);
	
	//basic enemy actions
	virtual void spawn() {};
	virtual void move() {};
	virtual void hit(float damage, Vec2Df _dir);
	virtual void die();


	//setter of scheduling parameters such as sawn delay, collidable flag ecc...
	void wobble(float dt);
	void spawnFly();
	virtual void trigger() override;

	virtual void update(float dt) override;
	virtual bool collidableWith(CollidableObject* obj) override;
	virtual bool collision(CollidableObject* with, Direction fromDir) override;
	virtual std::string name() override { return strprintf("Duke[%d]", _id); }
};