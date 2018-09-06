#pragma once
#include "FrameBuffer.h"
#include "Light.h"

/**
 * @enum	fogType
 * @brief	Represents the different types of fog processing.
 */

enum fogType { NO_FOG, LINEAR_FOG, EXPONENTIAL_FOG, EXPONENTIAL_2_FOG };

/**
 * @struct	FogParams
 * @brief	A fog parameters.
 */

struct FogParams {
	float start, end, density;
	fogType type;
	color color;
	float fogFactor(const glm::vec3 &fragPos, const glm::vec3 &eyePos) const;
};

/**
 * @struct	Fragment
 * @brief	Represents the information relevant to a single fragment. Think
 * 			of a fragment as a pixel competing to get into the framebuffer.
 */

struct Fragment {
	glm::vec3 windowPosition;
	Material material;
	glm::vec3 worldNormal;
	glm::vec3 worldPosition;
};

/**
 * @class	FragmentOps
 * @brief	Class to encapsulate the methods related to fragment processing.
 */

class FragmentOps {
	public:
		static bool performDepthTest;		//!< True ==> use depth buffer. Typically true
		static bool readonlyDepthBuffer;	//!< True ==> rendering will not affect depth buffer. Typically false
		static bool readonlyColorBuffer;	//!< True ==> rendering will not affect color buffer. Typically false
		static FogParams fogParams;			//!< Parameters controlling fog effects.
		static void FragmentOps::processFragment(FrameBuffer &frameBuffer, const glm::vec3 &eyePositionInWorldCoords,
														const std::vector<LightSourcePtr> lights, 
														const Fragment &fragment,
														const glm::mat4 &viewingMatrix);
	protected:
		static color FragmentOps::applyFog(const color &destColor,
											const glm::vec3 &eyePos, const glm::vec3 &fragPos);
		static color applyBlending(float alpha, const color &src, const color &dest);
		static color FragmentOps::applyLighting(const Fragment &fragment, const glm::vec3 &eyePositionInWorldCoords,
														const std::vector<LightSourcePtr> &lights,
														const glm::mat4 &viewingMatrix);
};
