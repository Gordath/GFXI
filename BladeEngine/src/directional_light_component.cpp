#include "directional_light_component.h"
#include "engine_context.h"

namespace Blade
{
	DirectionalLightComponent::DirectionalLightComponent(const struct DirectionalLightDesc& lightDesc, Entity* parent)
		: LightComponent{ LightType::DIRECTIONAL, parent },
		  m_LightDescription{ lightDesc }
	{
		G_LightSystem.RegisterComponent(this);
	}

	DirectionalLightComponent::~DirectionalLightComponent()
	{
		G_LightSystem.UnregisterComponent(GetId());
	}

	const DirectionalLightDesc& DirectionalLightComponent::GetLightDescription() const noexcept
	{
		return m_LightDescription;
	}

	DirectionalLightDesc* DirectionalLightComponent::GetLightDescriptionPtr() noexcept
	{
		return &m_LightDescription;
	}
}
