#pragma once

#include "Enemy.h"
#include <map>
#include <string>

namespace agp
{
	class Host;
}

class agp::Host : public Enemy
{
protected:

	std::map<std::string, Sprite*> _sprites;

	RenderableObject* _blood;
	RenderableObject* _bones;
	Direction _x_prev_dir;
	Direction _y_prev_dir;
	bool _canShoot;
	bool _shooting;
	bool _wobbling;
	float _accumulator;
	float _wobbleAccumulator;
	bool _wobbleTakeVar;
	float _fix_pos_x;
	float _fix_pos_y;
	float _acc;

public:

	Host(Scene* scene, const PointF& pos, float _spawnDelay);

	//basic enemy actions
	virtual void spawn() {};
	virtual void move() {};
	virtual void hit(float damage, Vec2Df _dir);
	virtual void die();

	virtual void update(float dt) override;

	void shoot();
	void wobble(float dt);
	virtual void trigger() override;

	virtual bool collidableWith(CollidableObject* obj) override;
	//virtual bool collision(CollidableObject* with, Direction fromDir) override;
	virtual std::string name() override { return strprintf("Duke[%d]", _id); }
};