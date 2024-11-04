#pragma once

#include "CollidableObject.h"
#include "geometryUtils.h"


namespace agp
{
	class Door;
	class DoorBorder;
	class DoorPanel;
	enum DoorState;
}

class agp::Door : public CollidableObject
{
	protected:

		Scene* _Scene;
		RoomType _type;
		DoorState _state;
		DoorPosition _pos;
		std::map<std::string, Sprite*> _sprites;
		Vec2Df _fixPos;
		Vec2Df _fixSize;
		bool _trigger;

		DoorBorder* _doorUp = nullptr;
		DoorBorder* _doorDown = nullptr;
		DoorBorder* _doorRight = nullptr;
		DoorBorder* _doorLeft = nullptr;

		DoorPanel* _doorUpLeft = nullptr;
		DoorPanel* _doorUpRight = nullptr;
		DoorPanel* _doorDownLeft = nullptr;
		DoorPanel* _doorDownRight = nullptr;
		DoorPanel* _doorRightLeft = nullptr;
		DoorPanel* _doorRightRight = nullptr;
		DoorPanel* _doorLeftLeft = nullptr;
		DoorPanel* _doorLeftRight = nullptr;

		RenderableObject* _doorLight;

	public:
		Door(Scene* scene, const RectF& rect, RoomType type, DoorPosition pos, float angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

		void Draw();

		void Trigger();

		virtual void update(float dt) override;

		virtual std::string name() override { return strprintf("Door[%d]", _id); }

};

class agp::DoorBorder : public RenderableObject
{
protected:

	Scene* _Scene;
	RoomType _type;
	DoorPosition _pos;
	std::map<std::string, Sprite*> _sprites;

	Vec2Df _fixPos;
	Vec2Df _fixSize;
	bool _trigger;
	int _bounceDirection;
	int _bounceCycle;

public:
	DoorBorder(Scene* scene, const RectF& rect, RoomType type, DoorPosition pos, int layer, float angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void Trigger();

	virtual void update(float dt) override;

	virtual std::string name() override { return strprintf("Border[%d]", _id); }

};

class agp::DoorPanel : public RenderableObject
{
protected:

	Scene* _Scene;
	RoomType _type;
	DoorState _state;
	DoorPosition _posDoor;
	PanelPosition _posPanel;
	std::map<std::string, Sprite*> _sprites;

	Vec2Df _fixPos;
	Vec2Df _fixSize;
	bool _open;
	bool _close;

public:
	DoorPanel(Scene* scene, const RectF& rect, RoomType type, DoorPosition posDoor, PanelPosition posPanel, int layer, float angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void Trigger();

	virtual void update(float dt) override;

	virtual std::string name() override { return strprintf("Panel[%d]", _id); }

};