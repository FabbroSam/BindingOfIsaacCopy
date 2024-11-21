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
	float accumulator;
	RenderableObject* _shadow;

public:
	
	Duke(Scene* scene, const PointF& pos, float _spawnDelay);
	
	//setter of scheduling parameters such as sawn delay, collidable flag ecc...
	void set_schedule_param();

	virtual void update(float dt) override;
	virtual bool collision(CollidableObject* with, Direction fromDir) override;
	virtual std::string name() override { return strprintf("Duke[%d]", _id); }
};