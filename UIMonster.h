
#pragma once
#include "UIScene.h"
#include "RenderableObject.h"
#include "MovableObject.h"
#include "Singleton.h"

namespace agp
{
	class UIMonster;
}

class agp::UIMonster : public Scene
{

		std::map<std::string, Sprite*> _sprites;

		bool _show;

		float _FPS;
		float _dx;

		MovableObject* _isaacspot;
		MovableObject* _isaac;
		MovableObject* _bossspot;
		MovableObject* _boss;
		MovableObject* _isaacname;
		MovableObject* _vs;
		MovableObject* _bossname;

	

	public:

		UIMonster();
		virtual ~UIMonster() {};

		bool show() { return _show; }
		void setActiveUIMonster();

		// extends update logic (+time management)
		virtual void update(float timeToSimulate) override;

		static UIMonster* UIMonsterBoss();
};