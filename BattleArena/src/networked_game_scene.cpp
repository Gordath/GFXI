#include "networked_game_scene.h"
#include "engine_context.h"
#include "game_scene_color_pass.h"
#include "directional_light.h"
#include "camera.h"
#include "windowing_service.h"
#include "d3d/D3D11_texture.h"
#include "mesh_utils.h"
#include "plane_collider.h"
#include "resource_utils.h"

#ifdef BLADE_BUILD_OVR
#include "game_scene_color_pass_ovr.h"
#endif

using namespace Blade;


NetworkedGameScene::NetworkedGameScene(const std::vector<PlayerInfo>& pInfos)
	: m_PlayerInfos{ pInfos }
{
}

bool NetworkedGameScene::Initialize()
{
//	for (auto player : m_Players)
//	{
//		BLADE_TRACE("Adding Player");
//		AddEntity(player);
//	}

	// Renderable Entity creation ----------------------------------------------------------------------------------------
	//Generate a Sphere.
	Mesh* cube{ MeshUtils::GenerateUvSphere(1.0f, 30, 30, 1.0f, 1.0f) };
	//Register the resource to the manager, so it manages it's lifetime(memory).
	G_ResourceManager.RegisterResource(cube, L"cube");

	Mesh* plane{ MeshUtils::GeneratePlaneXy(1.0f) };
	G_ResourceManager.RegisterResource(plane, L"plane");

	Texture* tex = G_ResourceManager.Get<D3D11Texture>(TEXTURE_PATH + L"star.jpg");

	//Define a material.
	Material material;
	material.diffuse = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	material.specular = Vec4f{ 1.0f, 1.0f, 1.0f, 60.0f }; //the w value is the shininess.

	Texture* diffuseTexture{ G_ResourceManager.Get<D3D11Texture>(TEXTURE_PATH + L"tunnelDiff5.png") };
	diffuseTexture->SetTextureType(TEX_DIFFUSE);
	material.textures[TEX_DIFFUSE] = diffuseTexture;

	Texture* specularTexture{ G_ResourceManager.Get<D3D11Texture>(TEXTURE_PATH + L"tunnelSpec5.png") };
	specularTexture->SetTextureType(TEX_SPECULAR);
	material.textures[TEX_SPECULAR] = specularTexture;

	Texture* normalmapTexture{ G_ResourceManager.Get<D3D11Texture>(TEXTURE_PATH + L"tunnelNorm5.png") };
	normalmapTexture->SetTextureType(TEX_NORMAL);
	material.textures[TEX_NORMAL] = normalmapTexture;
	//////////////////////////////////////////////////////////////////////////

	Entity* arena{ new Entity{ "arena" } };
	arena->Load(L"data/models/arena5.fbx");
	AddEntity(arena);
	m_WeaponFactory.SetArena(arena);
	Entity* entity{ new Entity{ "Environment" } };
	ColliderComponent* floor{ new ColliderComponent{ entity,std::make_unique<PlaneCollider>(Vec3f{ 0.0f,1.0f,0.0f },0.0f) } };
	ColliderComponent* wall1{ new ColliderComponent{ entity,std::make_unique<PlaneCollider>(Vec3f{ -1.0f,0.0f,0.0f },-40.0f) } };
	ColliderComponent* wall2{ new ColliderComponent{ entity,std::make_unique<PlaneCollider>(Vec3f{ 1.0f,0.0f,0.0f },-40.0f) } };
	ColliderComponent* wall3{ new ColliderComponent{ entity,std::make_unique<PlaneCollider>(Vec3f{ 0.0f,0.0f,1.0f },-40.0f) } };
	ColliderComponent* wall4{ new ColliderComponent{ entity,std::make_unique<PlaneCollider>(Vec3f{ 0.0f,0.0f,-1.0f },-40.0f) } };
	AddEntity(entity);

	int localPlayerNumber = std::atoi(&m_PlayerInfos[0].playerName.at(6));
	Player* p{ m_PlayerFactory.CreateKeyboardPlayer(m_PlayerInfos[0].playerName, m_PlayerInfos[0].fileName, localPlayerNumber, true) };
	p->SetPosition(m_PlayerInfos[0].spawnLocation);
	m_Players.push_back(p);
	AddEntity(p);

	for (int i{ 1 }; i<m_PlayerInfos.size();i++)
	{
		BLADE_TRACE("Adding Player");
		int playerNumber = std::atoi(&m_PlayerInfos[i].playerName.at(6));
		Player* p{ m_PlayerFactory.CreateJoypadPlayer(m_PlayerInfos[i].playerName, m_PlayerInfos[i].fileName, playerNumber, false) };
		p->SetPosition(m_PlayerInfos[i].spawnLocation);
		m_Players.push_back(p);
		AddEntity(p);
	}

	auto test{ GetEntityByName("player2") };


	m_WeaponFactory.GenerateWeapons();

	// Camera creation ---------------------------------------------------------------------------------------------------
	//Get the window size.
	Vec2i windowSize{ WindowingService::GetWindow(0)->GetSize() };

	CameraDesc cd;
	cd.viewport = Viewport{ Recti{ 0, 0, windowSize }, 0.0f, 1.0f };
	cd.nearPlane = 0.1f;
	cd.farPlane = 500.0f;
	cd.fov = MathUtils::ToRadians(45.0f);

	Camera* cam{ new Camera{ "Camera1", cd } };
	//Set the position of the camera.
	cam->SetPosition(Vec3f{ 0.0f, 60.0f, -70.0f });
	cam->SetOrientation(Vec3f{ 1.0f, 0.0f, 0.0f }, MathUtils::ToRadians(40.0f));

	//Add it to the scene.
	AddEntity(cam);

	cam = new Camera{ "Camera2", cd };
	cam->SetPosition(Vec3f{ 0.0f, 5.0f, -20.0f });
	AddEntity(cam);
	cam->SetParent(p);

	cam = new Camera{ "Camera3", cd };
	cam->SetPosition(Vec3f{ 0.0f, 10.0f, -50.0f });
	AddEntity(cam);


	cam = new Camera{ "Camera4", cd };

	cam->SetPosition(Vec3f{ 0.0f, 0.0f, -4.0f });
	cam->SetParent(m_Players[0]);
	AddEntity(cam);

	//Instruct the Camera system to set this camera as the active one.
	G_CameraSystem.SetActiveCamera("Camera3");


	// --------------------------------------------------------------------------------------------------------------------

	//Light Creation ------------------------------------------------------------------------------------------------------

	DirectionalLightDesc dlDesc;
	dlDesc.ambientIntensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	dlDesc.diffuseIntensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	dlDesc.specularIntensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };

	DirectionalLight* dl{ new DirectionalLight{ "DirectionalLight1", dlDesc } };

	dl->SetPosition(Vec3f{ 0.0f, 10.0f, -3.0f });

	AddEntity(dl);

	DirectionalLightDesc dlDesc2;
	dlDesc2.ambientIntensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	dlDesc2.diffuseIntensity = Vec4f{ 1.0f, 1.0f, 1.0f, 0.0f };
	dlDesc2.specularIntensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };

	dl = new DirectionalLight{ "DirectionalLight2", dlDesc2 };

	dl->SetPosition(Vec3f{ 0.0f, -10.0f, 0.0f });

	AddEntity(dl);
	// --------------------------------------------------------------------------------------------------------------------
	G_RenderSystem.SetSorting(true);
	// Pipeline Creation --------------------------------------------------------------------------------------------------
	//Allocate a render pass pipeline.
	RenderPassPipeline* pipeline{ new RenderPassPipeline };

#ifdef BLADE_BUILD_OVR
	ovrStage = new GameSceneColorPassStageOvr{ " ovrPass " };
	ovrStage->scale = 50.0f;
	if (!ovrStage->Initialize())
	{
		BLADE_ERROR("Failed to initialize the ovr pass stage.");
		return false;
	}

	pipeline->AddStage(ovrStage);
	G_CameraSystem.SetActiveCamera("Camera3");
	HealthBar::SetCurrentCamera();
#else
	//Allocate and initialize the a render pass pipeline stage.
	GameSceneColorPassStage* colorPassStage{ new GameSceneColorPassStage{ "GameSceneColorPass" } };
	if (!colorPassStage->Initialize())
	{
		BLADE_ERROR("Failed to initialize the color pass stage.");
		return false;
	}

	pipeline->AddStage(colorPassStage);
#endif

	//Set the pipeline to the render system.
	G_RenderSystem.SetRenderPassPipeline(pipeline);

	return true;
	// --------------------------------------------------------------------------------------------------------------------
}

void NetworkedGameScene::OnKeyDown(unsigned char key, int x, int y) noexcept
{
	switch (key)
	{
	case '1':
	{
#ifdef BLADE_BUILD_OVR
		ovrStage->scale = 50.0f;
#endif
		G_CameraSystem.SetActiveCamera("Camera1");
		HealthBar::SetCurrentCamera();
		G_AudioManager.PlaySample(ResourceUtils::GetAudioSample(L"ui_action.ogg"), 1.0f, AUDIO_PLAYMODE_ONCE);

	}
	break;
	case '2':
	{
#ifdef BLADE_BUILD_OVR
		ovrStage->scale = 5.0f;
#endif
		G_CameraSystem.SetActiveCamera("Camera2");
		HealthBar::SetCurrentCamera();
		G_AudioManager.PlaySample(ResourceUtils::GetAudioSample(L"ui_action.ogg"), 1.0f, AUDIO_PLAYMODE_ONCE);
	}
	break;
	case '3':
#ifdef BLADE_BUILD_OVR
		ovrStage->scale = 50.0f;
#endif
		G_CameraSystem.SetActiveCamera("Camera3");
		HealthBar::SetCurrentCamera();
		G_AudioManager.PlaySample(ResourceUtils::GetAudioSample(L"ui_action.ogg"), 1.0f, AUDIO_PLAYMODE_ONCE);
		break;
	default:
		break;
	}
}

void NetworkedGameScene::OnKeyUp(unsigned char key, int x, int y) noexcept
{
}

void NetworkedGameScene::OnMouseMotion(int x, int y) noexcept
{
}

void NetworkedGameScene::OnMouseClick(int button, bool state, int x, int y) noexcept
{
}

void NetworkedGameScene::Update(float deltaTime, long time) noexcept
{
	Scene::Update(deltaTime, time);

	G_InputManager.Update(deltaTime);

	G_SimulationSystem.Process(deltaTime);

	G_LightSystem.Process();

	G_BehaviourSystem.Process(deltaTime, time);


}

void NetworkedGameScene::Draw() const noexcept
{
	G_RenderSystem.Process();
}
