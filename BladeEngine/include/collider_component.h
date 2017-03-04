#ifndef BLADE_COLLIDER_COMPONENT_H_
#define BLADE_COLLIDER_COMPONENT_H_

#include "component.h"
#include "bounding_volume.h"
enum class BVolumeType
{
	Sphere, 
	Box, 
	Cylinder
};
namespace Blade
{
	class Entity;
	class ColliderComponent: public Component
	{

	private:
		bool m_CollisionResponse;
		BoundingVolume* m_pBVolume;
		bool m_ValidFlag;

	protected:
		void BVolume(BoundingVolume* bv) { m_pBVolume = bv; }
		BoundingVolume* BVolume() { return m_pBVolume; }

	public:
		ColliderComponent(Entity* parent);
		ColliderComponent(ColliderComponent&) = delete;
		ColliderComponent& operator=(ColliderComponent&) = delete;
		~ColliderComponent();

		bool Create(BVolumeType bVolumeType);
		bool isValid();


		/**************** WILL BE REMOVED ONCE COMPONENT DEFINITION STUFF IS SORTED **********/

		/**
		* \brief Performs setup actions after the Component's creation
		*/
		void Setup() noexcept override;

		/**
		* \brief Updates the Component on each frame.
		* \param dt The time elapsed from the previous frame of the Application.
		* \param time The elapsed time since the start of the Application.
		*/
		void Update(float dt, long time = 0) noexcept;

		/**
		* \brief Performs actions before the Component is destroyed.
		*/
		void Teardown() noexcept;

		/******************************************************************************/
	};
}
#endif //BLADE_COLLIDER_COMPONENT_H_