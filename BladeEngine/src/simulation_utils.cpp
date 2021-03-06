#include "simulation_utils.h"
#include "plane_collider.h"
#include "bounding_sphere.h"
#include "collider_component.h"
#include "entity.h"
#include <iostream>
namespace Blade
{

	bool SimulationUtils::SphereAndSphereCollision(const BoundingSphere * sphere1, const BoundingSphere * sphere2, ContactManifold & manifold) noexcept
	{
		Mat4f inv;
		Mat4f inv2;

		inv = MathUtils::Rotate(inv, sphere1->GetColliderComponent()->GetParent()->GetOrientation());
		inv = MathUtils::Inverse(inv);

		inv2 = MathUtils::Rotate(inv2, sphere2->GetColliderComponent()->GetParent()->GetOrientation());
		inv2 = MathUtils::Inverse(inv2);

		sphere1->GetColliderComponent()->GetParent()->CalculateXform();
		sphere2->GetColliderComponent()->GetParent()->CalculateXform();

		Mat4f f = sphere1->GetColliderComponent()->GetParent()->GetXform() * inv;
		Mat4f f1 = sphere2->GetColliderComponent()->GetParent()->GetXform() * inv2;

		Vec3f c1{ f * Vec4f{ 1.0f } };
		Vec3f c2{ f1 * Vec4f{ 1.0f } };

		Vec3f midline{ c1 - c2 };

		float size = MathUtils::Lengthf(midline);

		float radSum{ sphere1->GetRadius() + sphere2->GetRadius() };

		/*    if (size <= 0.0f || size >= radSum)
		{
		return false;
		}*/

		if (size > radSum)
		{
			return false;
		}
		//Vec3f normal{ midline * (1.0f / size) };

		Vec3f normal{ MathUtils::Normalize(midline) };

		ManifoldEntry entry;

		entry.contactNormal = normal;
		entry.collider1 = sphere1;
		entry.collider2 = sphere2;
		entry.penetration = radSum - size;

		manifold.AddEntry(entry);

		return true;
	}


	bool Blade::SimulationUtils::SphereAndPlaneCollision(const BoundingSphere * sphere, const PlaneCollider * plane, ContactManifold & manifold) noexcept
	{
		Vec3f planeDir{ plane->GetPlaneNormal() };
		float planeOffset{ plane->GetOffeset() };

		float sphereDistance{ MathUtils::Dotf(planeDir, sphere->GetColliderComponent()->GetParent()->GetLocalPosition()) - sphere->GetRadius() - planeOffset };

		if (sphereDistance <= 0.0f)
		{
			ManifoldEntry entry;
			entry.collider1 = sphere;
			entry.collider2 = nullptr;
			entry.contactNormal = planeDir;
			entry.penetration = -sphereDistance;

			manifold.AddEntry(entry);
			return true;
		}
		return false;
	}

	}