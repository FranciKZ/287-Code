#include "FragmentOps.h"

FogParams FragmentOps::fogParams;
bool FragmentOps::performDepthTest = true;
bool FragmentOps::readonlyDepthBuffer = false;
bool FragmentOps::readonlyColorBuffer = false;

/**
 * @fn	float FogParams::fogFactor(const glm::vec3 &fragPos, const glm::vec3 &eyePos) const
 * @brief	Computes fog factor - f.
 * @param	fragPos	The fragment position.
 * @param	eyePos 	The eye position.
 * @return	The fog factor - f.
 */

float FogParams::fogFactor(const glm::vec3 &fragPos, const glm::vec3 &eyePos) const {
	return 0.5f;
}

/**
 * @fn	color FragmentOps::applyLighting(const Fragment &fragment, const glm::vec3 &eyePositionInWorldCoords, const std::vector<LightSourcePtr> &lights, const glm::mat4 &viewingMatrix)
 * @brief	Applies the lighting to a fragment
 * @param	fragment					The fragment.
 * @param	eyePositionInWorldCoords	The eye position in world coordinates.
 * @param	lights						The vector of lights in the scene.
 * @param	viewingMatrix				The viewing matrix.
 *
 * @return	The color of the fragment after applying lighting equations.
 */

color FragmentOps::applyLighting(const Fragment &fragment, const glm::vec3 &eyePositionInWorldCoords,
										const std::vector<LightSourcePtr> &lights,
										const glm::mat4 &viewingMatrix) {
	return gray;
}

/**
 * @fn	color FragmentOps::applyFog(const color &destColor, const glm::vec3 &eyePos, const glm::vec3 &fragPos)
 * @brief	Applies fog to a fragment.
 * @param	destColor	Destination color.
 * @param	eyePos   	Eye position.
 * @param	fragPos  	Fragment position.
 * @return	The color after applying the fog.
 */

color FragmentOps::applyFog(const color &destColor,
							const glm::vec3 &eyePos, const glm::vec3 &fragPos) {
	return gray;
}

/**
 * @fn	color FragmentOps::applyBlending(float alpha, const color &srcColor, const color &destColor)
 * @brief	Applies blending to a fragment.
 * @param	alpha	 	Alpha value.
 * @param	srcColor 	Source color.
 * @param	destColor	Destination color.
 * @return	The blended color.
 */

color FragmentOps::applyBlending(float alpha, const color &srcColor, const color &destColor) {
	return gray;
}

/**
 * @fn	void FragmentOps::processFragment(FrameBuffer &frameBuffer, const glm::vec3 &eyePositionInWorldCoords, const std::vector<LightSourcePtr> lights, const Fragment &fragment, const glm::mat4 &viewingMatrix)
 * @brief	Process the fragment, leaving the results in the framebuffer.
 * @param [in,out]	frameBuffer					
 * @param 		  	eyePositionInWorldCoords	The eye position in world coordinates.
 * @param 		  	lights						Vector of lights in scene.
 * @param 		  	fragment					Fragment to be processed.
 * @param 		  	viewingMatrix				The viewing transformation matrix.
 */

void FragmentOps::processFragment(FrameBuffer &frameBuffer, const glm::vec3 &eyePositionInWorldCoords,
										const std::vector<LightSourcePtr> lights,
										const Fragment &fragment,
										const glm::mat4 &viewingMatrix) {
	const glm::vec3 &eyePos = eyePositionInWorldCoords;

	const float &Z = fragment.windowPosition.z;
	int X = (int)fragment.windowPosition.x;
	int Y = (int)fragment.windowPosition.y;
	DEBUG_PIXEL = (X == xDebug && Y == yDebug);
	bool passDepthTest = !performDepthTest || Z < frameBuffer.getDepth(X, Y);
	if (passDepthTest) {
		frameBuffer.setColor(X, Y, fragment.material.ambient);
		frameBuffer.setDepth(X, Y, Z);
	}
}