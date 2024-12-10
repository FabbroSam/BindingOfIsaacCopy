#pragma once

#include "DynamicObject.h"
#include "Scene.h"

namespace agp
{
	class Altar;
}

class agp::Altar : public DynamicObject {


private:
	float _x_dec_rel = 0.5f;
	float _y_dec_rel = 0.5f;
	float _pushVel = 0.5f;
	float _returnVel = 0.1f;

	float _radialDistance = 0;
	float _reactionForce = 5.0f;
	Vec2Df _originalPosition;

public:
	Altar(Scene* scene, const RectF& rect, Sprite* sprite, int layer = 0, float angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

	virtual bool collision(CollidableObject* with, Direction fromDir) override;
};
