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

	int _heart;
	std::map<std::string, Sprite*> _sprites;

	float _ampl = 0;
	float _freq = 0;
	float _accumulator;


public:

	Fly(Scene* scene, const PointF& pos, float _spawnDelay);

	virtual void set_schedule_param();

	virtual void hurt() override;


	virtual void update(float dt) override;
	virtual bool collidableWith(CollidableObject* obj) override;
	virtual bool collision(CollidableObject* with, Direction fromDir) override;
	virtual std::string name() override { return strprintf("Fly[%d]", _id); }
};