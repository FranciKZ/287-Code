#include "IScene.h"

/**
 * @fn	IScene::IScene(RaytracingCamera *theCamera ,bool showAxis)
 * @brief	Construct scene using a particular camera.
 * @param [in,out]	theCamera	The camera to use.
 * @param 		  	showAxis 	True to show, false to hide the xyz axes on the render.
 */

IScene::IScene(RaytracingCamera *theCamera, bool showAxis) {
	camera = theCamera;

	const float L = 20.0f;
	const float L2 = L / 2.0f;
	const float W = 0.1f;
	const float R = W/2;
	if (showAxis) {
//		IShapePtr xaxis = new ICylinderX(glm::vec3(L2, 0, 0), R, L);
		IShapePtr yaxis = new ICylinderY(glm::vec3(0, L2, 0), R, L);
//		IShapePtr zaxis = new ICylinderZ(glm::vec3(0, 0, L2), R, L);
//		visibleObjects.push_back(new VisibleIShape(xaxis, red));
		visibleObjects.push_back(new VisibleIShape(yaxis, green));
//		visibleObjects.push_back(new VisibleIShape(zaxis, blue));
	}
}

/**
 * @fn	void IScene::addObject(const VisibleIShapePtr &obj)
 * @brief	Adds an visible object to the scene
 * @param	obj	The object to be added.
 */

void IScene::addObject(const VisibleIShapePtr &obj) {
	visibleObjects.push_back(obj);
}

/**
 * @fn	void IScene::addTransparentObject(const VisibleIShapePtr &obj, float alpha)
 * @brief	Adds a transparent object to the scene
 * @param	obj  	The transparent object to be added.
 * @param	alpha	The alpha value of the object.
 */

void IScene::addTransparentObject(const VisibleIShapePtr &obj, float alpha) {
	obj->material.alpha = alpha;
	transparentObjects.push_back(obj);
}

/**
 * @fn	void IScene::addObject(const PositionalLightPtr &light)
 * @brief	Adds a positional light to the scene.
 * @param	light	The light to be added.
 */

void IScene::addObject(const PositionalLightPtr &light) {
	lights.push_back(light);
}

/**
 * @fn	void IScene::changeCamera(RaytracingCamera *cam)
 * @brief	Change camera
 * @param [in,out]	cam	The new camera.
 */

void IScene::changeCamera(RaytracingCamera *cam) {
	camera = cam;
}
