#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_
#include "scene.h"
#include "player_factory.h"
#include "weapon_factory.h"
#include "health_bar_factory.h"
#include "timer.h"


#ifdef BLADE_BUILD_OVR
class GameSceneColorPassStageOvr;
#else
class GameSceneColorPassStage;
#endif


class GameScene : public Blade::Scene
{
private:
	PlayerFactory m_PlayerFactory;
	WeaponFactory m_WeaponFactory;

	Blade::Material* m_pColumnMaterials[4] = { nullptr };
	Blade::Timer m_Timer;
	bool m_Fading{ false };

#ifdef BLADE_BUILD_OVR
	GameSceneColorPassStageOvr* ovrStage{ nullptr };
#else
	GameSceneColorPassStage* m_ColorPass{ nullptr };
#endif
	void FadeOutLogic(float deltaTime);

public:
	bool Initialize() override;

	void OnKeyDown(unsigned char key, int x, int y) noexcept override;

	void OnKeyUp(unsigned char key, int x, int y) noexcept override;

	void OnMouseMotion(int x, int y) noexcept override;

	void OnMouseClick(int button, bool state, int x, int y) noexcept override;

	void Update(float deltaTime, long time) noexcept override;

	void Draw() const noexcept override;
};

#endif //BATTLE_ARENA_GAME_SCENE_H_
