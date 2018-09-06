#include "Camera.h"

/**
 * @fn	RaytracingCamera::RaytracingCamera(const glm::vec3 &viewingPos, const glm::vec3 &lookAtPt, const glm::vec3 &up)
 * @brief	Constructs a raytracing camera.
 * @param	viewingPos	Location of camera.
 * @param	lookAtPt  	A focus point in front of the camera..
 * @param	up		  	Up vector.
 */

RaytracingCamera::RaytracingCamera(const glm::vec3 &viewingPos, const glm::vec3 &lookAtPt, const glm::vec3 &up) {
	changeConfiguration(viewingPos, lookAtPt, up);
}

/**
 * @fn	void RaytracingCamera::changeConfiguration(const glm::vec3 &viewingPos, const glm::vec3 &lookAtPt, const glm::vec3 &up)
 * @brief	Change configuration parameters of this camera.
 * @param	viewingPos	The new viewing position.
 * @param	lookAtPt  	A new focus point point.
 * @param	up		  	Up vector.
 */

void RaytracingCamera::changeConfiguration(const glm::vec3 &viewingPos, const glm::vec3 &lookAtPt, const glm::vec3 &up) {
	glm::vec3 u, v, w;
	cameraFrame.setFrame(viewingPos, u, v, w);
}

/**
 * @fn	PerspectiveCamera::PerspectiveCamera(const glm::vec3 &pos, const glm::vec3 &lookAtPt, const glm::vec3 &up, float FOVRads)
 * @brief	Constructs a perspective camera.
 * @param	pos			The position of the camera.
 * @param	lookAtPt	A focus point in front of the camera.
 * @param	up			Up vector.
 * @param	FOVRads 	The field of view in radians.
 */

PerspectiveCamera::PerspectiveCamera(const glm::vec3 &pos, const glm::vec3 &lookAtPt, const glm::vec3 &up, float FOVRads)
	: RaytracingCamera(pos, lookAtPt, up) {
	fov = FOVRads;
}

/**
 * @fn	OrthographicCamera::OrthographicCamera(const glm::vec3 &pos, const glm::vec3 &lookAtPt, const glm::vec3 &up, float ppwu)
 * @brief	Constructs an orthographic camera.
 * @param	pos			Position of camera.
 * @param	lookAtPt	A focus point in front of the camera.
 * @param	up			Up vector.
 * @param	ppwu		Pixels per world unit.
 */

OrthographicCamera::OrthographicCamera(const glm::vec3 &pos, const glm::vec3 &lookAtPt, const glm::vec3 &up, float ppwu)
	: RaytracingCamera(pos, lookAtPt, up) {
	pixelsPerWorldUnit = ppwu;
}

/**
 * @fn	glm::vec2 RaytracingCamera::getProjectionPlaneCoordinates(float x, float y) const
 * @brief	Gets projection plane coordinates at (x, y).
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @return	Projection plane coordinates.
 */

glm::vec2 RaytracingCamera::getProjectionPlaneCoordinates(float x, float y) const {
	glm::vec2 s;
	return s;
}

/**
 * @fn	void PerspectiveCamera::calculateViewingParameters(int W, int H)
 * @brief	Calculates the viewing parameters associated with this camera.
 * @param	W	The width of window.
 * @param	H	The height of window.
 */

void PerspectiveCamera::calculateViewingParameters(int W, int H) {
	// fill in nx, ny, distToPlane, top, bottom, left, and right
}

/**
 * @fn	void OrthographicCamera::calculateViewingParameters(int W, int H)
 * @brief	Calculates the viewing parameters associated with this camera.
 * @param	W	The width of window.
 * @param	H	The height of window.
 */

void OrthographicCamera::calculateViewingParameters(int W, int H) {
	// fill in nx, ny, top, bottom, left, and right
}

/**
 * @fn	Ray OrthographicCamera::getRay(float x, float y) const
 * @brief	Determines camera ray going through projection plane at (x, y), in direction -w.
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @return	The ray through the projection plane at (x, y), in direction -w.
 */

Ray OrthographicCamera::getRay(float x, float y) const {
	glm::vec2 uv = getProjectionPlaneCoordinates(x, y);
	return Ray(cameraFrame.origin + uv.x * cameraFrame.u + uv.y * cameraFrame.v, -cameraFrame.w);
}

/**
 * @fn	Ray PerspectiveCamera::getRay(float x, float y) const
 * @brief	Determines ray eminating from camera through the projection plane at (x, y).
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @return	The ray eminating from camera through the projection plane at (x, y).
 */

Ray PerspectiveCamera::getRay(float x, float y) const {
	glm::vec2 uv = getProjectionPlaneCoordinates(x, y);
	glm::vec3 rayDirection = glm::normalize((float)(-distToPlane) * cameraFrame.w +
																	uv.x * cameraFrame.u + 
																	uv.y * cameraFrame.v); // Page 76
	return Ray(cameraFrame.origin, rayDirection);
}

/**
 * @fn	void PerspectiveCamera::setFOV(float FOV, int W, int H)
 * @brief	Sets a camera's field of view.
 * @param	FOV	The field of view.
 * @param	W  	Width of window.
 * @param	H  	Height of window.
 */

void PerspectiveCamera::setFOV(float FOV, int W, int H) {
	fov = FOV;
	calculateViewingParameters(W, H);
}

/**
* @fn	ostream &operator << (std::ostream &os, const RaytracingCamera &camera)
* @brief	Output stream for cameras.
* @param	os		Output stream.
* @param	camera  The camera.
*/

std::ostream &operator << (std::ostream &os, const RaytracingCamera &camera) {
	os << "Camera info:" << std::endl;
	os << "Frame" << std::endl;
	os << camera.cameraFrame << std::endl;
	return os;
}
