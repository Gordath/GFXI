#include <algorithm>
#include "SystemSimulation.h"


namespace Blade
{

	SystemSimulation::~SystemSimulation()
	{
	}

	bool SystemSimulation::Initialize() noexcept
	{
		return true;
	}

	void SystemSimulation::Process(float deltaTime) noexcept
	{
		//TO DO: Simulation code
		
	}

	void SystemSimulation::RegisterComponent(RigidBodyComponent* rbc) noexcept
	{
		m_RigidBodyList.push_back(rbc);
	}

	void SystemSimulation::RegisterComponent(ColliderComponent* col) noexcept
	{
		m_ColliderList.push_back(col);
	}

	void SystemSimulation::UnregisterComponent(Component* c) noexcept
	{
	
		//m_RigidBodyList.erase(std::remove(m_RigidBodyList.begin(), m_RigidBodyList.end(), c), m_RigidBodyList.end()); - not used
		
		// TO BE CHANGED..
		
		auto positionIteratorRB = std::find(m_RigidBodyList.begin(), m_RigidBodyList.end(), c);
		if (positionIteratorRB != m_RigidBodyList.end()) // element found!
		{
			m_RigidBodyList.erase(positionIteratorRB);
		}
		else //not in rigidbodies.. look for it in colliders
		{
			auto positionIteratorCOL = std::find(m_ColliderList.begin(), m_ColliderList.end(), c);
			//if found... remove it from the list
			if (positionIteratorCOL != m_ColliderList.end())  m_ColliderList.erase(positionIteratorCOL);
		}

	}
}