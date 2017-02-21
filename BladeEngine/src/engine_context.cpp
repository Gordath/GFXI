#include "engine_context.h"
#include "d3d/D3D11_context.h"
#include <iostream>
#include "render_state_manager.h"

namespace Blade
{
#if defined(BLADE_BUILD_D3D)
	D3D11Context* EngineContext::m_GAPIContext{ nullptr };
#else
#endif

	std::unique_ptr<RenderSystem> EngineContext::m_RenderSystem{ std::make_unique<RenderSystem>()};

	EngineContext::~EngineContext()
	{
		delete m_GAPIContext;
	}

	bool EngineContext::Initialize()
	{
#if defined(BLADE_BUILD_D3D)
		m_GAPIContext = new D3D11Context;
#else
		//Allocate GL context
#endif
		if (!m_GAPIContext->Create())
		{
			std::cerr << "Failed to initialize the engine's Graphics Context!" << std::endl;
			return false;
		}

		//TODO: Initialise the systems here
		if (!m_RenderSystem->Initialize())
		{
			std::cerr << "Failed to initialize the RenderSystem." << std::endl;
			return false;
		}

		RenderStateManager::Initialize();

		return true;
	}

	RenderSystem* EngineContext::GetRenderSystem() noexcept
	{
		return m_RenderSystem.get();
	}
}
