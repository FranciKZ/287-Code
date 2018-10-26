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
// send out viewing ray from intercept point to light position
// check if the ray hits anything
// then check if the hit is between the light and the lightposition (distance(intercept, second intercept) < distance(intercept, lightPos)
// must change this as well
color RayTracer::traceIndividualRay(const Ray &ray, const IScene &theScene, int recursionLevel) const {
	HitRecord theHit = VisibleIShape::findIntersection(ray, theScene.visibleObjects);
	HitRecord transHit = VisibleIShape::findIntersection(ray, theScene.transparentObjects);
	color result = defaultColor;
	bool inShadow = false;
	if (theHit.t < FLT_MAX) {
		if (glm::dot(ray.direction, theHit.surfaceNormal) > 0) {
			theHit.surfaceNormal = -theHit.surfaceNormal;
		}
		shadowFeeler(ray, theScene, recursionLevel, inShadow, theHit, 0);
		if (theHit.texture != nullptr) {
			float u = glm::clamp(theHit.u, 0.0f, 1.0f);
			float v = glm::clamp(theHit.v, 0.0f, 1.0f);
			result = theHit.texture->getPixel(u, v); +
				theScene.lights[0]->illuminate(theHit.interceptPoint,
					theHit.surfaceNormal, theHit.material, 
					theScene.camera->cameraFrame, inShadow);
		}
		else {
			result = theScene.lights[0]->illuminate(theHit.interceptPoint, theHit.surfaceNormal,
				theHit.material, theScene.camera->cameraFrame, inShadow);
		}
		if (transHit.t < FLT_MAX) {
			float opaqueHitDist = glm::distance(theHit.interceptPoint, theScene.lights[0]->lightPosition);
			float transHitDist = glm::distance(transHit.interceptPoint, theScene.lights[0]->lightPosition);
			if (opaqueHitDist > transHitDist) {
				color transHitColor = theScene.lights[0]->illuminate(transHit.interceptPoint,
					transHit.surfaceNormal, transHit.material, theScene.camera->cameraFrame, inShadow);
				color finalColor = (1 - transHit.material.alpha) * result + (transHit.material.alpha) * transHitColor;
				result = finalColor;
			}
		}
	}
	else if (theHit.t == FLT_MAX && transHit.t < FLT_MAX) {
		color transHitColor = theScene.lights[0]->illuminate(transHit.interceptPoint,
			transHit.surfaceNormal, transHit.material, theScene.camera->cameraFrame, inShadow);
		color finalColor = (1 - transHit.material.alpha) * defaultColor + (transHit.material.alpha) * transHitColor;
		result = finalColor;
	}
	return result;
}

void RayTracer::shadowFeeler(const Ray &ray, const IScene &theScene, int recursionLevel, bool &inShadow, HitRecord theHit, int i) const {
	glm::vec3 shadowFeelerOrig = (theHit.interceptPoint + EPSILON * theHit.surfaceNormal);
	Ray shadowFeeler(shadowFeelerOrig, pointingVector(shadowFeelerOrig, theScene.lights[i]->lightPosition));
	HitRecord shadowHit = VisibleIShape::findIntersection(shadowFeeler, theScene.visibleObjects);
	if (shadowHit.t < FLT_MAX) {
		float distToLight = glm::distance(shadowFeeler.origin, theScene.lights[i]->lightPosition);
		float distToHit = glm::distance(shadowFeeler.origin, shadowHit.interceptPoint);
		if (distToHit < distToLight) {
			inShadow = true;
		}
		else {
			inShadow = false;
		}
	}
}