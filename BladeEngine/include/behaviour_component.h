#ifndef BLADE_BEHAVIOUR_COMPONENT_H_
#define BLADE_BEHAVIOUR_COMPONENT_H_
#include "component.h"
#include "trace.h"
namespace Blade
{
	/**
	 * \brief BehaviourComponent class of the engine. All the components of the engine
	 * that needs to perform and action derive from this class. BehaviourComponent inherits from the Component class so it can
	 * register and receive specific messages.
	*/
	class BehaviourComponent : public Component
	{
	public:
		/**
		* \brief Component constructor.
		* \param type The type of the Component as a string.
		* \param parent The Entity the Component will be attached to.
		*/
		BehaviourComponent(const std::string& type, Entity* parent);

		BehaviourComponent(const BehaviourComponent& other) = delete;

		BehaviourComponent& operator=(const BehaviourComponent& other) = delete;

		virtual ~BehaviourComponent();

		/**
		* \brief Updates the Component on each frame.
		* \param dt The time elapsed from the previous frame of the Application.
		* \param time The elapsed time since the start of the Application.
		*/
		virtual void Update(const float dt, const long time = 0) noexcept = 0;

		/**
		* \brief Performs setup actions after the BehaviourComponent's creation
		*/
		virtual void Setup() noexcept = 0;

		/**
		* \brief Performs actions before the BehaviourComponent is destroyed.
		*/
		virtual void Teardown() noexcept = 0;


		/**
		* \brief Method called when collision between parent of this component and other enitity is detected.
		* \param other Pointer to other that was part of the collision.
		* \remarks In order for the collision to be detected, both entities need to have colliders.
		*/
		virtual void OnCollision(Entity* other) noexcept { }
	};
}

#endif