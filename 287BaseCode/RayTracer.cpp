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

// add boolean for two vantage points
// go through another nested forloop if true
void RayTracer::raytraceScene(FrameBuffer &frameBuffer, int depth,
								const IScene &theScene, int aaValue) const {
	const RaytracingCamera &camera = *theScene.camera;
	const std::vector<VisibleIShapePtr> &objs = theScene.visibleObjects;
	const std::vector<PositionalLightPtr> &lights = theScene.lights;
	// where major changes for raytracing project will be
	if (aaValue == 1) {
		for (int y = 0; y < frameBuffer.getWindowHeight(); ++y) {
			for (int x = 0; x < frameBuffer.getWindowWidth(); ++x) {
				Ray ray = camera.getRay((float)x, (float)y);
				color colorForPixel = traceIndividualRay(ray, theScene, depth);
				frameBuffer.setColor(x, y, colorForPixel);
			}
		}
	}
	if (aaValue == 3) {
		for (int y = 0; y < frameBuffer.getWindowHeight(); ++y) {
			for (int x = 0; x < frameBuffer.getWindowWidth(); ++x) {
				Ray ray = camera.getRay((float)x, (float)y);
				Ray leftMid = camera.getRay((float)x - EPSILON * 500, (float)y);
				Ray rightMid = camera.getRay((float)x + EPSILON * 500, (float)y);
				Ray upMid = camera.getRay((float)x, (float)y + EPSILON * 500);
				Ray downMid = camera.getRay((float)x, (float)y + EPSILON * 500);
				Ray upLeft = camera.getRay((float)x - EPSILON * 500, (float)y + EPSILON * 500);
				Ray upRight = camera.getRay((float)x + EPSILON * 500, (float)y + EPSILON * 500);
				Ray botLeft = camera.getRay((float)x - EPSILON * 500, (float)y - EPSILON * 500);
				Ray botRight = camera.getRay((float)x + EPSILON * 500, (float)y - EPSILON * 500);
				
				color colorForPixel = traceIndividualRay(ray, theScene, depth);
				color colorForLeftMid = traceIndividualRay(leftMid, theScene, depth);
				color colorForRightMid = traceIndividualRay(rightMid, theScene, depth);
				color colorForUpMid = traceIndividualRay(upMid, theScene, depth);
				color colorForDownMid = traceIndividualRay(downMid, theScene, depth);
				color colorForUpLeft = traceIndividualRay(upLeft, theScene, depth);
				color colorForUpRight = traceIndividualRay(upRight, theScene, depth);
				color colorForBotLeft = traceIndividualRay(botLeft, theScene, depth);
				color colorForBotRight = traceIndividualRay(botRight, theScene, depth);
				color finalPixelColor = (colorForPixel + colorForLeftMid + colorForRightMid +
										colorForUpMid + colorForDownMid + colorForUpLeft + 
										colorForUpRight + colorForBotLeft + colorForBotRight);
				finalPixelColor = finalPixelColor / 9.0f;
				frameBuffer.setColor(x, y, finalPixelColor);
			}
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
	bool inShadow1 = false;
	bool inShadow2 = false;
	if (theHit.t < FLT_MAX) {
		// backsurface rendering
		if (glm::dot(ray.direction, theHit.surfaceNormal) > 0) {
			theHit.surfaceNormal = -theHit.surfaceNormal;
		}
		if (theScene.lights[0]->isOn) {
			shadowFeeler(ray, theScene, recursionLevel, inShadow1, theHit, 0);
		}
		if (theScene.lights[1]->isOn) {
			shadowFeeler(ray, theScene, recursionLevel, inShadow2, theHit, 1);
		}
		color posColor = posColor = theScene.lights[0]->illuminate(theHit.interceptPoint, theHit.surfaceNormal,
			theHit.material, theScene.camera->cameraFrame, inShadow1);
		color spotColor = theScene.lights[1]->illuminate(theHit.interceptPoint, theHit.surfaceNormal,
			theHit.material, theScene.camera->cameraFrame, inShadow2);
		if (theHit.texture != nullptr) {
			float u = glm::clamp(theHit.u, 0.0f, 1.0f);
			float v = glm::clamp(theHit.v, 0.0f, 1.0f);//50/50 mapping of texture
			result = (0.5f) * theHit.texture->getPixel(u, v) + (0.25f) * posColor + (0.25f) * spotColor;
		}
		else {
			result = posColor + spotColor;
		}
		if (transHit.t < FLT_MAX) {
			float opaqueHitDist = glm::distance(theHit.interceptPoint, theScene.lights[0]->lightPosition);
			float transHitDist = glm::distance(transHit.interceptPoint, theScene.lights[0]->lightPosition);
			if (opaqueHitDist > transHitDist) {
				color transHitColor = theScene.lights[0]->illuminate(transHit.interceptPoint,
					transHit.surfaceNormal, transHit.material, theScene.camera->cameraFrame, false);
				color transSpotHitColor = theScene.lights[1]->illuminate(transHit.interceptPoint,
					transHit.surfaceNormal, transHit.material, theScene.camera->cameraFrame, false);
				color finalColorPos = (1 - transHit.material.alpha) * result + (transHit.material.alpha) * transHitColor;
				color finalColorSpot = (1 - transHit.material.alpha) * result + (transHit.material.alpha) * transSpotHitColor;
				result = finalColorPos + finalColorSpot;
			}
		}
		if (recursionLevel > 0) {
			glm::vec3 reflectRayOrig = (theHit.interceptPoint + EPSILON * theHit.surfaceNormal);
			Ray reflectRay(reflectRayOrig, (ray.direction - 2 * glm::dot(ray.direction, theHit.surfaceNormal)*theHit.surfaceNormal));
			if (!theScene.lights[0]->isOn && !theScene.lights[1]->isOn) {
				result = black;
			}
			else {
				result = (0.8f) * result + (0.2f) * traceIndividualRay(reflectRay, theScene, recursionLevel - 1);
			}
					
		}
	}
	else if (theHit.t == FLT_MAX && transHit.t < FLT_MAX) {
		color transHitColor = theScene.lights[0]->illuminate(transHit.interceptPoint,
			transHit.surfaceNormal, transHit.material, theScene.camera->cameraFrame, inShadow1);
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
		if(shadowHit.material.alpha == 1.0f){
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
}