
#pragma once
#include "UIScene.h"
#include "RenderableObject.h"
#include "MovableObject.h"
#include "Singleton.h"

namespace agp
{
	class UIMonster;
}

class agp::UIMonster : public UIScene, public Singleton<UIMonster>
{
	friend class Singleton<UIMonster>;

		std::map<std::string, Sprite*> _sprites;

		bool _show;

		int _FPS;
		float _dx;

		MovableObject* _isaacspot;
		MovableObject* _isaac;
		MovableObject* _bossspot;
		MovableObject* _boss;
		MovableObject* _isaacname;
		MovableObject* _vs;
		MovableObject* _bossname;

		UIMonster();

	public:

		virtual ~UIMonster() {};
	
		void showVS();
		bool getShowVS() { return _show; }

		// extends update logic (+time management)
		virtual void update(float timeToSimulate) override;
};