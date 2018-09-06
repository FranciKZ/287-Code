#pragma once

#include <vector>
#include "defs.h"
#include "ColorAndMaterials.h"
#include "Image.h"
#include "Utilities.h"

/**
 * @struct	HitRecord
 * @brief	Stores information regarding a ray-object intersection. Used in raytracing.
 */

struct HitRecord {
	float t;					//!< the t value where the intersection took place.
	glm::vec3 interceptPoint;	//!< the (x,y,z) value where the intersection took place.
	glm::vec3 surfaceNormal;	//!< the normal vector at the intersection point.
	Material material;			//!< the Material value of the object.
	Image *texture;				//!< the texture associated with this object, if any.
	float u, v;					//!< (u,v) correpsonding to intersection point.

	/**
	 * @fn	HitRecord()
	 * @brief	Constructs a HitRecord that corresponds to "no hit"
	 */

	HitRecord() {
		t = FLT_MAX;
		texture = nullptr; 
	}

	/**
	 * @fn	static HitRecord getClosest(const std::vector<HitRecord> &hits)
	 * @brief	Gets a closest, give a vector of hits.
	 * @param	hits	The hits to consider.
	 * @return	The closest hit, that is in front of the camera.
	 */

	static HitRecord getClosest(const std::vector<HitRecord> &hits) {
		HitRecord theHit;
		for (unsigned int i = 0; i < hits.size(); i++) {
			if (inRangeExclusive(hits[i].t, 0, theHit.t)) {
				theHit = hits[i];
			}
		}
		return theHit;
	}
};
