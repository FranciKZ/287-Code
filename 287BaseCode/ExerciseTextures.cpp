#include <ctime>
#include <utility>
#include <cctype>
#include <ctime> 

#include "ColorAndMaterials.h"
#include "FrameBuffer.h"
#include "IScene.h"
#include "IShape.h"
#include "RayTracer.h"
#include "Camera.h"
#include "Image.h"
#include <ctime>
#include <utility>
#include <cctype>
#include <ctime> 

FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
Image im("usflag.ppm");
//Image im("snail.ppm");
//Image im("squares.ppm");
//Image im("star_field.ppm");

float angle = 0.0f;
bool isAnimated = true;

glm::vec3 cameraPos(0, 0, 10);
glm::vec3 lookAt = ORIGIN3D;
glm::vec3 up = Y_AXIS;

PerspectiveCamera camera(cameraPos, lookAt, up, (float)M_PI_2);
IScene theScene(&camera, true);

RayTracer rayTrace(white);

PositionalLightPtr posLight = new PositionalLight(glm::vec3(-10.0f, 5.0f, 15.0f), pureWhiteLight);

void buildScene() {
	IShapePtr cylinder = new ICylinderY(glm::vec3(0, 0, 0), 6.0f, 15.0f);
	//IShapePtr cylinder = new ISphere(glm::vec3(0, 0, 0), 6);
	VisibleIShapePtr p;
	theScene.addObject(p = new VisibleIShape(cylinder, gold));
	p->setTexture(&im);
	theScene.addObject(posLight);
}

void render() {
	int frameStartTime = glutGet(GLUT_ELAPSED_TIME);

	float R = 15;
	float rads = glm::radians(angle);
	cameraPos = glm::vec3(R*std::cos(-rads), R, R*std::sin(-rads));
	camera.changeConfiguration(cameraPos, lookAt, up);
	rayTrace.raytraceScene(frameBuffer, 0, theScene);
	int frameEndTime = glutGet(GLUT_ELAPSED_TIME);
	float totalTimeSec = (frameEndTime - frameStartTime) / 1000.0f;

	std::cout << "Render time: " << totalTimeSec << " sec." << std::endl;
}

void resize(int width, int height) {
	frameBuffer.setFrameBufferSize(width, height);
	camera.calculateViewingParameters(width, height);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (std::toupper(key)) {
	case 'P':
		isAnimated = !isAnimated;
		break;
	case ESCAPE:
		glutLeaveMainLoop();
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}
	glutPostRedisplay();
}

void timer(int id) {
	glutTimerFunc(TIME_INTERVAL, timer, 0);
	if (!isAnimated) return;
	angle += 5.0f;
	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	GLuint world_Window = glutCreateWindow(__FILE__);

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(TIME_INTERVAL, timer, 0);
	glutMouseFunc(mouseUtility);

	buildScene();

	glutMainLoop();
	return 0;
}