#include <ctime> 
#include <iostream>
#include <vector>
#include "EShape.h"
#include "Light.h"
#include "VertexOps.h"

PositionalLightPtr theLight = new PositionalLight(glm::vec3(2, 1, 3), pureWhiteLight);
std::vector<LightSourcePtr> lights = { theLight };

const double WIDTH = 10.0;
const int DIV = 20;

glm::vec3 position(0, 1, 5);
float angle = 0;
bool isMoving = true;
bool twoViewOn = false;
const float SPEED = 0.1f;

FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

EShapeData plane = EShape::createECheckerBoard(copper, polishedCopper, 10, 10, 10);
EShapeData cone1 = EShape::createECone(gold, 2.0, 1.0, 20, 20);
EShapeData cone2 = EShape::createECone(brass, 0.5, 0.5, 10, 10);
EShapeData cyl1 = EShape::createECylinder(silver, 0.5, 1, 20);
EShapeData cyl2 = EShape::createECylinder(silver, 0.5, 1, 20);

void renderObjects() {
	VertexOps::render(frameBuffer, plane, lights, glm::mat4());
	VertexOps::render(frameBuffer, cone1, lights, T(-1, 2, 0)*S(0.25f)*Rx(angle));
	VertexOps::render(frameBuffer, cone2, lights, Ry(angle)*T(2, 1, 0)*Rx(angle));
	VertexOps::render(frameBuffer, cyl1, lights, T(2,1,0));
	VertexOps::render(frameBuffer, cyl2, lights, T(-2, 1, 0)*Rx(M_PI_2));
}

static void render() {
	int frameStartTime = glutGet(GLUT_ELAPSED_TIME);

	frameBuffer.clearColorAndDepthBuffers();
	int width = frameBuffer.getWindowWidth();
	int height = frameBuffer.getWindowHeight();
	VertexOps::viewingTransformation = glm::lookAt(position, ORIGIN3D, Y_AXIS);
	float AR = (float)width / height;
	VertexOps::projectionTransformation = glm::perspective(M_PI_3, AR, 0.5f, 80.0f);
	VertexOps::setViewport(0, width - 1, 0, height - 1);
	renderObjects();
	frameBuffer.showColorBuffer();

	int frameEndTime = glutGet(GLUT_ELAPSED_TIME); // Get end time
	double totalTimeSec = (frameEndTime - frameStartTime) / 1000.0;
	std::cout << "Render time: " << totalTimeSec << " sec." << std::endl;
}

void resize(int width, int height) {
	frameBuffer.setFrameBufferSize(width, height);
	float AR = (float)width / height;

	VertexOps::setViewport(0, width - 1, 0, height - 1);
	VertexOps::projectionTransformation = glm::perspective(M_PI_3, AR, 0.5f, 80.0f);

	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
	const float INC = 0.5f;
	switch (key) {
	case 'X':
	case 'x': theLight->lightPosition.x += (isupper(key) ? INC : -INC);
				std::cout << theLight->lightPosition << std::endl;
				break;
	case 'Y':
	case 'y': theLight->lightPosition.y += (isupper(key) ? INC : -INC);
				std::cout << theLight->lightPosition << std::endl;
				break;
	case 'Z':
	case 'z': theLight->lightPosition.z += (isupper(key) ? INC : -INC);
				std::cout << theLight->lightPosition << std::endl;
				break;
	case 'P':
	case 'p':	isMoving = !isMoving;
				break;
	case 'C':
	case 'c':	break;
	case '?':	twoViewOn = !twoViewOn;
				break;
	case ESCAPE:
				glutLeaveMainLoop();
				break;
	default:
				std::cout << (int)key << "unmapped key pressed." << std::endl;
	}

	glutPostRedisplay();
}

static void timer(int id) {
	if (isMoving) {
		angle += glm::radians(5.0f);
	}
	glutTimerFunc(100, timer, 0);
	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	GLuint world_Window = glutCreateWindow(extractBaseFilename(__FILE__).c_str());
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(100, timer, 0);
	glutMouseFunc(mouseUtility);
	frameBuffer.setClearColor(lightGray);

	glutMainLoop();

	return 0;
}