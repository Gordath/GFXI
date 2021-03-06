#include "render_component.h"
#include "engine_context.h"

namespace Blade
{
	RenderComponent::RenderComponent(Entity* parent)
		: Component{ "co_render", parent }
	{
		G_RenderSystem.RegisterComponent(this);
	}

	RenderComponent::~RenderComponent()
	{
		G_RenderSystem.UnregisterComponent(GetId());
	}

	Mesh* RenderComponent::GetMesh() const noexcept
	{
		return m_Mesh;
	}

	void RenderComponent::SetMesh(Mesh* mesh) noexcept
	{
		m_Mesh = mesh;
	}

	const Material& RenderComponent::GetMaterial() const noexcept
	{
		return m_Material;
	}

	void RenderComponent::SetMaterial(const Material& material) noexcept
	{
		m_Material = material;
	}

}
