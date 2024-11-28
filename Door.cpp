#include "Door.h"
#include "SpriteFactory.h"
#include "StaticObject.h"
#include "Scene.h"
#include "Audio.h"
#include <string>
#include <list>

using namespace agp;


Door::Door(Scene* scene, const RectF& rect, RoomType type, DoorPosition pos, float angle, SDL_RendererFlip flip)
	: StaticObject(scene, rect, nullptr)
{

	_scene = scene;
	_angle = angle;
	_flip = flip;
	_type = type;
	_pos = pos;
	_state = DoorState::OPEN;
	_compenetrable = true;
	_sprite = nullptr;
	_visible = true;
	_fixPos = rect.pos;
	_fixSize = rect.size;
	_trigger = false;

	_doorUp = nullptr;
	_doorDown = nullptr;
	_doorRight = nullptr;
	_doorLeft = nullptr;
	_doorUpLeft = nullptr;
	_doorUpRight = nullptr;
	_doorDownLeft = nullptr;
	_doorDownRight = nullptr;
	_doorRightLeft = nullptr;
	_doorRightRight = nullptr;
	_doorLeftLeft = nullptr;
	_doorLeftRight = nullptr;
	_doorLight = nullptr;


	_sprites["normal"] = SpriteFactory::instance()->get("door_normal");
	_sprites["normal_back"] = SpriteFactory::instance()->get("door_normal_back");
	_sprites["treasure"] = SpriteFactory::instance()->get("door_treasure");
	_sprites["boss"] = SpriteFactory::instance()->get("door_boss");
	_sprites["boss_back"] = SpriteFactory::instance()->get("door_boss_back");
	_sprites["boss_light"] = SpriteFactory::instance()->get("door_boss_light");

	Draw();
	
}

void Door::Draw()
{
	std::string door;
	std::string backdoor;

	if (_type == RoomType::BOSS)
	{
		if (_pos == DoorPosition::TOP)
		{
			_doorUp = new DoorBorder(_scene, RectF(_rect.pos.x, _rect.pos.y + 0.1f, _rect.size.x, _rect.size.y), _type, _pos, 2, _angle, _flip);
			_doorUpLeft = new DoorPanel(_scene, RectF(_rect.pos.x + 0.2f, _rect.pos.y + 0.1f, _rect.size.x - 1, _rect.size.y), _type, _pos, PanelPosition::LEFT, 1, _angle, _flip);
			_doorUpRight = new DoorPanel(_scene, RectF(_rect.pos.x + 0.8f, _rect.pos.y + 0.1f, _rect.size.x - 1, _rect.size.y), _type, _pos, PanelPosition::RIGHT, 1, _angle, _flip);
			_doorLight = new RenderableObject(_scene, RectF(_rect.pos.x + 0.2f, _rect.pos.y + 2, 1.5f, 1), _sprites["boss_light"], 5, _angle, _flip);
		}
		else if (_pos == DoorPosition::BOTTOM)
		{
			_doorDown = new DoorBorder(_scene, RectF(_rect.pos.x, _rect.pos.y - 0.1f, _rect.size.x, _rect.size.y), _type, _pos, 2, _angle, _flip);
			_doorDownLeft = new DoorPanel(_scene, RectF(_rect.pos.x + 0.2f, _rect.pos.y - 0.1f, _rect.size.x - 1, _rect.size.y), _type, _pos, PanelPosition::LEFT, 1, _angle, _flip);
			_doorDownRight = new DoorPanel(_scene, RectF(_rect.pos.x + 0.8f, _rect.pos.y - 0.1f, _rect.size.x - 1, _rect.size.y), _type, _pos, PanelPosition::RIGHT, 1, _angle, _flip);
			_doorLight = new RenderableObject(_scene, RectF(_rect.pos.x + 0.23f, _rect.pos.y - 1, 1.5f, 1), _sprites["boss_light"], 5, _angle, _flip);
		}
		else if (_pos == DoorPosition::RIGHT)
		{
			_doorRight = new DoorBorder(_scene, RectF(_rect.pos.x - 0.1f, _rect.pos.y, _rect.size.x, _rect.size.y - 0.2f), _type, _pos, 2, _angle, _flip);
			_doorRightLeft = new DoorPanel(_scene, RectF(_rect.pos.x - 0.1f, _rect.pos.y + 0.85f, 2, 1), _type, _pos, PanelPosition::LEFT, 1, 0, SDL_FLIP_HORIZONTAL);
			_doorRightRight = new DoorPanel(_scene, RectF(_rect.pos.x - 0.1f, _rect.pos.y + 0.34f, 2, 1), _type, _pos, PanelPosition::RIGHT, 1, 0, SDL_FLIP_HORIZONTAL);
			_doorLight = new RenderableObject(_scene, RectF(_rect.pos.x - 1.2f, _rect.pos.y + 0.6f, 1.5f, 1), _sprites["boss_light"], 5, _angle, _flip);
		}
		else if (_pos == DoorPosition::LEFT)
		{
			_doorLeft = new DoorBorder(_scene, RectF(_rect.pos.x + 0.1f, _rect.pos.y, _rect.size.x, _rect.size.y - 0.2f), _type, _pos, 2, _angle, _flip);
			_doorLeftLeft = new DoorPanel(_scene, RectF(_rect.pos.x + 0.1f, _rect.pos.y + 0.89f, 2, 1), _type, _pos, PanelPosition::LEFT, 1);
			_doorLeftRight = new DoorPanel(_scene, RectF(_rect.pos.x + 0.1f, _rect.pos.y + 0.38f, 2, 1), _type, _pos, PanelPosition::RIGHT, 1);
			_doorLight = new RenderableObject(_scene, RectF(_rect.pos.x + 1.7f, _rect.pos.y + 0.68f, 1.5f, 1), _sprites["boss_light"], 5, _angle, _flip);
		}	
	}
	else
	{
		if (_pos == DoorPosition::TOP)
		{
			_doorUp = new DoorBorder(_scene, RectF(_rect.pos.x, _rect.pos.y + 0.1f, _rect.size.x, _rect.size.y), _type, _pos, 2, _angle, _flip);
			_doorUpLeft = new DoorPanel(_scene, RectF(_rect.pos.x - 0.1f, _rect.pos.y + 0.1f, _rect.size.x - 0.5f, _rect.size.y), _type, _pos, PanelPosition::LEFT, 1, _angle, _flip);
			_doorUpRight = new DoorPanel(_scene, RectF(_rect.pos.x + 0.66f, _rect.pos.y + 0.1f, _rect.size.x - 0.5f, _rect.size.y), _type, _pos, PanelPosition::RIGHT, 1, _angle, _flip);
		}
		else if (_pos == DoorPosition::BOTTOM)
		{
			_doorDown = new DoorBorder(_scene, RectF(_rect.pos.x, _rect.pos.y - 0.1f, _rect.size.x, _rect.size.y), _type, _pos, 2, _angle, _flip);
			_doorDownLeft = new DoorPanel(_scene, RectF(_rect.pos.x - 0.1f, _rect.pos.y - 0.1f, _rect.size.x - 0.5f, _rect.size.y + 0.1f), _type, _pos, PanelPosition::LEFT, 1, _angle, _flip);
			_doorDownRight = new DoorPanel(_scene, RectF(_rect.pos.x + 0.66f, _rect.pos.y - 0.1f, _rect.size.x - 0.5f, _rect.size.y + 0.1f), _type, _pos, PanelPosition::RIGHT, 1, _angle, _flip);
		}
		else if (_pos == DoorPosition::RIGHT)
		{
			_doorRight = new DoorBorder(_scene, RectF(_rect.pos.x - 0.1f, _rect.pos.y, _rect.size.x, _rect.size.y - 0.2f), _type, _pos, 2, _angle, _flip);
			_doorRightLeft = new DoorPanel(_scene, RectF(_rect.pos.x - 0.1f, _rect.pos.y + 0.85f, 2, 1), _type, _pos, PanelPosition::LEFT, 1, 0, SDL_FLIP_HORIZONTAL);
			_doorRightRight = new DoorPanel(_scene, RectF(_rect.pos.x - 0.1f, _rect.pos.y + 0.34f, 2, 1), _type, _pos, PanelPosition::RIGHT, 1, 0, SDL_FLIP_HORIZONTAL);
		}
		else if (_pos == DoorPosition::LEFT)
		{
			_doorLeft = new DoorBorder(_scene, RectF(_rect.pos.x + 0.1f, _rect.pos.y, _rect.size.x, _rect.size.y - 0.2f), _type, _pos, 2, _angle, _flip);
			_doorLeftLeft = new DoorPanel(_scene, RectF(_rect.pos.x + 0.1f, _rect.pos.y + 0.89f, 2, 1), _type, _pos, PanelPosition::LEFT, 1);
			_doorLeftRight = new DoorPanel(_scene, RectF(_rect.pos.x + 0.1f, _rect.pos.y + 0.38f, 2, 1), _type, _pos, PanelPosition::RIGHT, 1);
		}
	}
}

void Door::openClose()
{

	if (_doorUp)
		_doorUp->Trigger();
	else if (_doorDown)
		_doorDown->Trigger();
	else if (_doorRight)
		_doorRight->Trigger();
	else if (_doorLeft)
		_doorLeft->Trigger();

	schedule("TriggerPanel", 0.4f, [this]()
		{
			if (_state == DoorState::OPEN)
				Audio::instance()->playSound("door_open");
			else if (_state == DoorState::ClOSE)
				Audio::instance()->playSound("door_close");

			if (_doorUp) {
				_doorUpLeft->Trigger();
				_doorUpRight->Trigger();
			}
			else if (_doorDown) {
				_doorDownLeft->Trigger();
				_doorDownRight->Trigger();
			}
			else if (_doorRight) {
				_doorRightLeft->Trigger();
				_doorRightRight->Trigger();
			}
			else if (_doorLeft) {
				_doorLeftLeft->Trigger();
				_doorLeftRight->Trigger();
			}
		}, 0);

	schedule("ToggleLightDoor", 0.6f, [this]()
		{
			if (_doorLight && (_state == DoorState::ClOSE))
				_doorLight->setVisible(false);
			else if (_doorLight && _state == DoorState::OPEN)
				_doorLight->setVisible(true);
		}, 0);

	if (_state == DoorState::OPEN)
	{
		_compenetrable = false;
		_state = DoorState::ClOSE;
	}
	else if (_state == DoorState::ClOSE)
	{
		_compenetrable = true;
		_state = DoorState::OPEN;
	}
}

void Door::offLight()
{
	if (_doorLight)
		_doorLight->setVisible(false);
}

void Door::update(float dt)
{
	CollidableObject::update(dt);
}

DoorBorder::DoorBorder(Scene* scene, const RectF& rect, RoomType type, DoorPosition pos, int layer, float angle, SDL_RendererFlip flip)
	: RenderableObject(scene, rect, nullptr, layer, angle, flip)
{

	_scene = scene;
	_rect = rect;
	_angle = angle;
	_flip = flip;
	_layer = layer;

	_type = type;
	_pos = pos;
	_fixPos = rect.pos;
	_fixSize = rect.size;
	_trigger = false;
	_bounceDirection = 1;
	_bounceCycle = 0;

	_sprites["normal"] = SpriteFactory::instance()->get("door_normal");
	_sprites["normal_back"] = SpriteFactory::instance()->get("door_normal_back");
	_sprites["treasure"] = SpriteFactory::instance()->get("door_treasure");
	_sprites["boss"] = SpriteFactory::instance()->get("door_boss");
	_sprites["boss_back"] = SpriteFactory::instance()->get("door_boss_back");

	if (_type == RoomType::BOSS)
	{
		_sprite = _sprites["boss"];

		if (_pos == DoorPosition::TOP)
			new RenderableObject(_scene, RectF(_rect.pos.x - 0.2f, _rect.pos.y - 0.35f, _rect.size.x + 0.4f, _rect.size.y + 0.4f), _sprites["boss_back"], 1, _angle, _flip);
		else if (_pos == DoorPosition::BOTTOM)
			new RenderableObject(_scene, RectF(_rect.pos.x - 0.2f, _rect.pos.y - 0.1f, _rect.size.x + 0.4f, _rect.size.y + 0.4f), _sprites["boss_back"], 1, _angle, _flip);
		else if (_pos == DoorPosition::RIGHT)
			new RenderableObject(_scene, RectF(_rect.pos.x - 0, _rect.pos.y - 0.2f, _rect.size.x + 0.4f, _rect.size.y + 0.4f), _sprites["boss_back"], 1, _angle, _flip);
		else if (_pos == DoorPosition::LEFT)
			new RenderableObject(_scene, RectF(_rect.pos.x - 0.4f, _rect.pos.y - 0.2f, _rect.size.x + 0.4f, _rect.size.y + 0.4f), _sprites["boss_back"], 1, _angle, _flip);
	}
	else 
	{
		if (_type == RoomType::TREASURE)
			_sprite = _sprites["treasure"];
		else
			_sprite = _sprites["normal"];

		if (_pos == DoorPosition::TOP)
			new RenderableObject(_scene, RectF(_rect.pos.x - 0.6f, _rect.pos.y - 0.45f, _rect.size.x + 1.3f, _rect.size.y + 0.7f), _sprites["normal_back"], 1, _angle, _flip);
		else if (_pos == DoorPosition::BOTTOM)
			new RenderableObject(_scene, RectF(_rect.pos.x - 0.65f, _rect.pos.y - 0.2f, _rect.size.x + 1.3f, _rect.size.y + 0.7f), _sprites["normal_back"], 1, _angle, _flip);
		else if (_pos == DoorPosition::RIGHT)
			new RenderableObject(_scene, RectF(_rect.pos.x - 0.5f, _rect.pos.y - 0.35f, _rect.size.x + 1.3f, _rect.size.y + 0.7f), _sprites["normal_back"], 1, _angle, _flip);
		else if (_pos == DoorPosition::LEFT)
			new RenderableObject(_scene, RectF(_rect.pos.x - 0.75f, _rect.pos.y - 0.35f, _rect.size.x + 1.3f, _rect.size.y + 0.7f), _sprites["normal_back"], 1, _angle, _flip);
		
	}

}

void DoorBorder::Trigger()
{
	_trigger = true;
}

void DoorBorder::update(float dt)
{
	RenderableObject::update(dt);

	if (_trigger)
	{
		if (_pos == DoorPosition::TOP)
		{
			if (_rect.pos.x <= _fixPos.x + 0.3f && _rect.pos.x >= _fixPos.x - 0.3f)
			{
				_rect.pos.x -= 0.025f * _bounceDirection;
				_rect.pos.y -= 0.025f * _bounceDirection;
				_rect.size.x += 0.05f * _bounceDirection;
				_rect.size.y += 0.025f * _bounceDirection;
				if (_rect.size.y >= _fixSize.y + 0.18f || _rect.size.y < _fixSize.y)
				{
					_bounceDirection *= -1;
					_bounceCycle += 1;

					if (_bounceCycle > 1)
					{
						_trigger = false;
						_rect.size = _fixSize; 
						_rect.pos = _fixPos;  
						_bounceCycle = 0;

					}
				}
			}
		}
		else if (_pos == DoorPosition::BOTTOM)
		{
			if (_rect.pos.x <= _fixPos.x + 0.3f && _rect.pos.x >= _fixPos.x - 0.3f)
			{
				_rect.pos.x -= 0.025f * _bounceDirection;
				_rect.size.x += 0.05f * _bounceDirection;
				_rect.size.y += 0.025f * _bounceDirection;
				if (_rect.size.y >= _fixSize.y + 0.18f || _rect.size.y < _fixSize.y)
				{
					_bounceDirection *= -1;
					_bounceCycle += 1;

					if (_bounceCycle > 1)
					{
						_trigger = false;
						_rect.size = _fixSize;
						_rect.pos = _fixPos;
						_bounceCycle = 0;

					}
				}
			}
		}
		else if (_pos == DoorPosition::RIGHT)
		{
			if (_rect.pos.x <= _fixPos.x + 0.3f && _rect.pos.x >= _fixPos.x - 0.3f)
			{
				_rect.pos.x -= 0.015f * _bounceDirection;
				_rect.size.x += 0.05f * _bounceDirection;
				_rect.size.y += 0.025f * _bounceDirection;
				if (_rect.size.y >= _fixSize.y + 0.18f || _rect.size.y < _fixSize.y)
				{
					_bounceDirection *= -1;
					_bounceCycle += 1;

					if (_bounceCycle > 1)
					{
						_trigger = false;
						_rect.size = _fixSize;
						_rect.pos = _fixPos;
						_bounceCycle = 0;

					}
				}
			}
		}
		else if (_pos == DoorPosition::LEFT)
		{
			if (_rect.pos.x <= _fixPos.x + 0.4f && _rect.pos.x >= _fixPos.x - 0.4f)
			{
				_rect.pos.x -= 0.025f * _bounceDirection;
				_rect.pos.y -= 0.025f * _bounceDirection;
				_rect.size.y += 0.05f * _bounceDirection;
				if (_rect.size.y >= _fixSize.y + 0.36f || _rect.size.y < _fixSize.y)
				{
					_bounceDirection *= -1;
					_bounceCycle += 1;

					if (_bounceCycle > 1)
					{
						_trigger = false;
						_rect.size = _fixSize;
						_rect.pos = _fixPos;
						_bounceCycle = 0;

					}
				}
			}
		}
	}
}

DoorPanel::DoorPanel(Scene* scene, const RectF& rect, RoomType type, DoorPosition posDoor, PanelPosition posPanel, int layer, float angle, SDL_RendererFlip flip)
	: RenderableObject(scene, rect, nullptr, layer, angle, flip)
{
	_scene = scene;
	_rect = rect;
	_angle = angle;
	_flip = flip;
	_layer = layer;
	_visible = false;

	_type = type;
	_posDoor = posDoor;
	_posPanel = posPanel;
	_fixPos = rect.pos;
	_fixSize = rect.size;
	_open = false;
	_close = false;	
	_state = DoorState::ClOSE;
	
	_sprites["boss_panel_left"] = SpriteFactory::instance()->get("door_boss_panel_left");
	_sprites["boss_panel_right"] = SpriteFactory::instance()->get("door_boss_panel_right");
	_sprites["boss_panel_left_rotation"] = SpriteFactory::instance()->get("door_boss_panel_left_rotation");
	_sprites["boss_panel_right_rotation"] = SpriteFactory::instance()->get("door_boss_panel_right_rotation");
	_sprites["normal_panel_left"] = SpriteFactory::instance()->get("door_normal_panel_left");
	_sprites["normal_panel_right"] = SpriteFactory::instance()->get("door_normal_panel_right");
	_sprites["normal_panel_left_rotation"] = SpriteFactory::instance()->get("door_normal_panel_left_rotation");
	_sprites["normal_panel_right_rotation"] = SpriteFactory::instance()->get("door_normal_panel_right_rotation");

	if (_type == RoomType::BOSS)
	{
		if (_posDoor == DoorPosition::TOP || _posDoor == DoorPosition::BOTTOM)
		{
			if (_posPanel == PanelPosition::LEFT)
				_sprite = _sprites["boss_panel_left"];
			else if (_posPanel == PanelPosition::RIGHT)
				_sprite = _sprites["boss_panel_right"];
		}
		else if (_posDoor == DoorPosition::RIGHT || _posDoor == DoorPosition::LEFT)
		{
			if (_posPanel == PanelPosition::LEFT)
				_sprite = _sprites["boss_panel_left_rotation"];
			else if (_posPanel == PanelPosition::RIGHT)
				_sprite = _sprites["boss_panel_right_rotation"];
		}
	}
	else
	{
		if (_posDoor == DoorPosition::TOP || _posDoor == DoorPosition::BOTTOM)
		{
			if (_posPanel == PanelPosition::LEFT)
				_sprite = _sprites["normal_panel_left"];
			else if (_posPanel == PanelPosition::RIGHT)
				_sprite = _sprites["normal_panel_right"];
		}
		else if (_posDoor == DoorPosition::RIGHT || _posDoor == DoorPosition::LEFT)
		{
			if (_posPanel == PanelPosition::LEFT)
				_sprite = _sprites["normal_panel_left_rotation"];
			else if (_posPanel == PanelPosition::RIGHT)
				_sprite = _sprites["normal_panel_right_rotation"];
		}
	}

	Trigger();
}

void DoorPanel::Trigger()
{

	if (_state == DoorState::OPEN)
	{		
		_close = true;
		_visible = true;
		_state = DoorState::ClOSE;
	}
	else if (_state == DoorState::ClOSE)
	{
		_open = true;
		_state = DoorState::OPEN;
	}
}

void DoorPanel::update(float dt)
{
	RenderableObject::update(dt);

	if (_posDoor == DoorPosition::TOP || _posDoor == DoorPosition::BOTTOM)
	{
		if (_posPanel == PanelPosition::LEFT)
		{
			if (_open)
			{
				_rect.size.x -= 0.03f;
				_rect.pos.x += 0.01f;
				if (_rect.size.x < 0.6)
				{
					_open = false;
					_visible = false;
				}
			}
			if (_close)
			{
				_rect.size.x += 0.03f;
				_rect.pos.x -= 0.01f;
				if (_rect.size.x > _fixSize.x)
				{
					_rect.pos = _fixPos;
					_rect.size = _fixSize;
					_close = false;
				}
			}
		}
		else if (_posPanel == PanelPosition::RIGHT)
		{
			if (_open)
			{
				_rect.pos.x += 0.02f;
				_rect.size.x -= 0.035f;		
				if (_rect.size.x < 0.6)
				{
					_open = false;
					_visible = false;
				}
			}
			if (_close)
			{
				_rect.pos.x -= 0.02f;
				_rect.size.x += 0.036f;
				if (_rect.pos.x < _fixPos.x)
				{
					_rect.pos = _fixPos;
					_rect.size = _fixSize;
					_close = false;
				}
			}
		}
	}
	else if (_posDoor == DoorPosition::LEFT || _posDoor == DoorPosition::RIGHT)
	{
		if (_posPanel == PanelPosition::LEFT)
		{
			if (_open)
			{
				_rect.pos.y += 0.023f;
				_rect.size.y -= 0.034f;
				if (_rect.size.y < 0.1)
				{
					_open = false;
					_visible = false;
				}
			}
			if (_close)
			{
				_rect.pos.y -= 0.023f;
				_rect.size.y += 0.036f;
				if (_rect.pos.y < _fixPos.y)
				{
					_rect.pos = _fixPos;
					_rect.size = _fixSize;
					_close = false;
				}
			}

		}
		else if (_posPanel == PanelPosition::RIGHT)
		{
			if (_open)
			{
				_rect.size.y -= 0.024f;
				_rect.pos.y += 0.003f;
				if (_rect.size.y < 0.6)
				{
					_open = false;
					_visible = false;
				}
			}
			if (_close)
			{
				_rect.size.y += 0.024f;
				_rect.pos.y -= 0.003f;
				if (_rect.size.y > _fixSize.y)
				{
					_rect.pos = _fixPos;
					_rect.size = _fixSize;
					_close = false;
				}
			}
		}
	}

}