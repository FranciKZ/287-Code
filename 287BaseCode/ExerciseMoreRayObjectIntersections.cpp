#include <ctime>
#include "Defs.h"
#include "IShape.h"
#include "FrameBuffer.h"
#include "Raytracer.h"
#include "IScene.h"
#include "Light.h"
#include "Image.h"
#include "Camera.h"
#include "Rasterization.h"

int currLight = 0;
float z = 0.0f;
float inc = 0.2f;

std::vector<PositionalLightPtr> lights = {
	new PositionalLight(glm::vec3(-10, 10, 10), pureWhiteLight),
	new SpotLight(glm::vec3(2, 5, -2), glm::vec3(0,-1,0), glm::radians(45.0f), pureWhiteLight)
};

PositionalLightPtr posLight = lights[0];
SpotLightPtr spotLight = (SpotLightPtr)lights[1];

FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
RayTracer rayTrace(lightGray);
PerspectiveCamera pCamera(glm::vec3(0, 5, 10), glm::vec3(0, 5, 0), Y_AXIS, (float)M_PI_2);
OrthographicCamera oCamera(glm::vec3(0, 5, 10), glm::vec3(0, 5, 0), Y_AXIS, 25.0f);
RaytracingCamera *cameras[] = { &pCamera, &oCamera };
int currCamera = 0;
IScene scene(cameras[currCamera], true);

void render() {
	int frameStartTime = glutGet(GLUT_ELAPSED_TIME);

	float N = 10.0f;
	cameras[currCamera]->changeConfiguration(glm::vec3(0, 5, 10), glm::vec3(0, 0, 0), Y_AXIS);
	rayTrace.raytraceScene(frameBuffer, 0, scene);

	int frameEndTime = glutGet(GLUT_ELAPSED_TIME); // Get end time
	float totalTimeSec = (frameEndTime - frameStartTime) / 1000.0f;
	std::cout << "Render time: " << totalTimeSec << " sec." << std::endl;
}

void resize(int width, int height) {
	frameBuffer.setFrameBufferSize(width, height);
	cameras[currCamera]->calculateViewingParameters(width, height);
	glutPostRedisplay();
	std::cout << cameras[currCamera]->getProjectionPlaneCoordinates(0, 0) << std::endl;
}

void buildScene() {
	IShape *bronzeDisk = new IDisk(glm::vec3(-6, 0, -5), glm::vec3(0,0,1), 3);
	IShape *tinDisk = new IDisk(glm::vec3(-8, 0, -4), glm::vec3(0, 0, 1), 3);
	ICylinderY *cylinder = new ICylinderY(glm::vec3(4.0f, 0.0f, -1.0f), 5.0f, 3.0f);

	scene.addObject(new VisibleIShape(bronzeDisk, bronze));
	scene.addObject(new VisibleIShape(tinDisk, tin));
	scene.addObject(new VisibleIShape(cylinder, gold));

	scene.addObject(lights[0]);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case ESCAPE:
		glutLeaveMainLoop();
		break;
	default:
		std::cout << (int)key << "unmapped key pressed." << std::endl;
	}

	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	GLuint world_Window = glutCreateWindow(__FILE__);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseUtility);
	buildScene();

	rayTrace.defaultColor = gray;
	glutMainLoop();

	return 0;
}