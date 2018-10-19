#include "RayTracer.h"
#include "IShape.h"

/**
 * @fn	RayTracer::RayTracer(const color &defa)
 * @brief	Constructs a raytracers.
 * @param	defa	The clear color.
 */

RayTracer::RayTracer(const color &defa)
	: defaultColor(defa) {
}

/**
 * @fn	void RayTracer::raytraceScene(FrameBuffer &frameBuffer, int depth, const IScene &theScene) const
 * @brief	Raytrace scene
 * @param [in,out]	frameBuffer	Framebuffer.
 * @param 		  	depth	   	The current depth of recursion.
 * @param 		  	theScene   	The scene.
 */

void RayTracer::raytraceScene(FrameBuffer &frameBuffer, int depth,
								const IScene &theScene) const {
	const RaytracingCamera &camera = *theScene.camera;
	const std::vector<VisibleIShapePtr> &objs = theScene.visibleObjects;
	const std::vector<PositionalLightPtr> &lights = theScene.lights;
	// where major changes for raytraceing project will be
	for (int y = 0; y < frameBuffer.getWindowHeight(); ++y) {
		for (int x = 0; x < frameBuffer.getWindowWidth(); ++x) {
			Ray ray = camera.getRay((float)x, (float)y);
			color colorForPixel = traceIndividualRay(ray, theScene, depth);
			frameBuffer.setColor(x, y, colorForPixel);
		}
	}

	frameBuffer.showColorBuffer();
}

/**
 * @fn	color RayTracer::traceIndividualRay(const Ray &ray, const IScene &theScene, int recursionLevel) const
 * @brief	Trace an individual ray.
 * @param	ray			  	The ray.
 * @param	theScene	  	The scene.
 * @param	recursionLevel	The recursion level.
 * @return	The color to be displayed as a result of this ray.
 */

// IScene contains stuff like cameras, lights, and objects
// Hit record contains normal vector, material, intercept point (t)
color RayTracer::traceIndividualRay(const Ray &ray, const IScene &theScene, int recursionLevel) const {
	HitRecord theHit = VisibleIShape::findIntersection(ray, theScene.visibleObjects);
	color result = defaultColor;

	// must change this as well
	if (theHit.t < FLT_MAX) {
		// call illuminate or totalColor
		// this is where lighting equations must be calculated
		if (theHit.texture != nullptr) { // if has texture then do this
			float u = glm::clamp(theHit.u, 0.0f, 1.0f);
			float v = glm::clamp(theHit.v, 0.0f, 1.0f);
			result = theHit.texture->getPixel(u, v) + 
				theScene.lights[0]->illuminate(theHit.interceptPoint, theHit.surfaceNormal, theHit.material, theScene.camera->cameraFrame, false);
		}
		else { // else comput
			// add shadow feelers to determine if hit is in shadow
			result = theScene.lights[0]->illuminate(theHit.interceptPoint, theHit.surfaceNormal, theHit.material, theScene.camera->cameraFrame, false);
		}		
	}
	return result;
}