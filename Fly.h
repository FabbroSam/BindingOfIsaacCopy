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

	//game parameters
	bool _black; //type
	float _distFromIsaac;
	bool _duke_fly;
	float _accumulator;

public:

	Fly(Scene* scene, const PointF& pos, float _spawnDelay, bool duke_fly);
	
	//basic enemy actions
	virtual void spawn() {};
	virtual void move();
	void moveAroundDuke(Vec2Df _target, Vec2Df fly, float dt);
	virtual void hit(float damage, Vec2Df _dir = { 0,0 });
	virtual void die();

	void followIsaac(Vec2Df pos);

	virtual void update(float dt) override;
	virtual bool collidableWith(CollidableObject* obj) override;
	virtual std::string name() override { return strprintf("Fly[%d]", _id); }
};