#include "Lift.h"

using namespace agp;

Lift::Lift(Scene* scene, const RectF& rect, Sprite* sprite, bool vertical, float range, int layer) :
	KinematicObject(scene, rect, sprite, layer)
{
	_compenetrable = false;
	_vertical = vertical;
	_r0 = (_vertical ? _rect.pos.y : _rect.pos.x) - range / 2.0f;
	_r1 = (_vertical ? _rect.pos.y : _rect.pos.x) + range / 2.0f;

	if (_vertical)
	{
		_y_vel_max = 3;
		_y_vel_min = 0;
	}
	else
	{
		_x_acc = 3.f;
		_x_dec_skd = 3.f;
		_x_vel_max = 3;
		move_x(Direction::RIGHT);
	}
}

void Lift::update(float dt)
{

	KinematicObject::update(dt);
}