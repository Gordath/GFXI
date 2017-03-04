#ifndef BLADE_BOUNDING_VOLUME_H_
#define BLADE_BOUNDING_VOLUME_H_

#include "mesh.h"
namespace Blade
{
	/**
	* \brief Common base class (and an interface) for all bounding shape classes.
	*/
	class BoundingVolume
	{

	private:
		//TO DO: Add all relevant members
	public:
		BoundingVolume(Mesh* mesh);
		BoundingVolume(BoundingVolume&) = delete;
		BoundingVolume& operator=(BoundingVolume&) = delete;
		virtual ~BoundingVolume();

		

	};
}
#endif //BLADE_BOUNDING_VOLUME_H_

