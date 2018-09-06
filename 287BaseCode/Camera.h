#pragma once
#include <iostream>
#include "IShape.h"

/**
 * @struct	RaytracingCamera
 * @brief	Base class for cameras in raytracing applications.
 */

struct RaytracingCamera {
	Frame cameraFrame;					//!< The camera's frame
	float fov;							//!< The camera's field of view
	float left, right, bottom, top;		//!< The camera's field of view
	float nx, ny;						//!< Window size
	RaytracingCamera(const glm::vec3 &pos, const glm::vec3 &lookAtPt, const glm::vec3 &up);
	void changeConfiguration(const glm::vec3 &pos, const glm::vec3 &lookAtPt, const glm::vec3 &up);
	glm::vec2 getProjectionPlaneCoordinates(float x, float y) const;
	virtual void calculateViewingParameters(int width, int height) = 0;
	virtual Ray getRay(float x, float y) const = 0;
	friend std::ostream &operator << (std::ostream &os, const RaytracingCamera &camera);
};

/**
 * @struct	PerspectiveCamera
 * @brief	Encapsulates a perspective camera for raytracing applications.
 */

struct PerspectiveCamera : public RaytracingCamera {
	float distToPlane;				//!< Distance to image plane
	PerspectiveCamera(const glm::vec3 &pos, const glm::vec3 &lookAtPt, const glm::vec3 &up, float FOVRads);
	virtual void calculateViewingParameters(int width, int height);
	virtual Ray getRay(float x, float y) const;
	void setFOV(float FOV, int W, int H);
};

/**
 * @struct	OrthographicCamera
 * @brief	Encapsulates a orthographic camera for raytracing applications.
 */

struct OrthographicCamera : public RaytracingCamera {
	float pixelsPerWorldUnit;		//!< Controls the size of the image plane.
	OrthographicCamera(const glm::vec3 &pos, const glm::vec3 &lookAtPt, const glm::vec3 &up, float ppwu);
	virtual void calculateViewingParameters(int width, int height);
	virtual Ray getRay(float x, float y) const;
};