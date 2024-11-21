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

	virtual void set_schedule_param();

	virtual void update(float dt) override;
	virtual bool collision(CollidableObject* with, Direction fromDir) override;
	virtual std::string name() override { return strprintf("Fly[%d]", _id); }
};