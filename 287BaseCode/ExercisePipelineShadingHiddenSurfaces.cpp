#include <ctime> 
#include <iostream>
#include <vector>
#include "EShape.h"
#include "Light.h"
#include "VertexOps.h"

PositionalLightPtr theLight = new PositionalLight(glm::vec3(0, 10, 4), pureWhiteLight);
std::vector<LightSourcePtr> lights = { theLight };
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

EShapeData board = EShape::createECheckerBoard(copper, tin, 10, 10, 10);

// sphere
float f(float x, float y) {
	float x2 = x * x;
	float y2 = y * y;
	if (x2 + y2 > 1)
		return -1.0f;
	else
		return std::sqrt(1.0f - x*x - y*y);
}

glm::vec3 n(float x, float z) {
	glm::vec3 N(x, f(x, z), z);
	glm::vec3 normedN = glm::normalize(N);
	return normedN;
}

EShapeData createSurface(const Material &mat) {
	EShapeData verts;
	const float INC = 0.05f;
	const float W = 1;
	for (float x=-W; x<W; x+=INC) {			// simplistic algorithm
		for (float z=-W; z<W; z+=INC) {		// could be better
			float xLeft = x;
			float zLeft = z;
			float xRight = xLeft + INC;
			float zRight = zLeft + INC;

			VertexData ll(glm::vec4(xLeft, f(xLeft, zLeft), zLeft, 1.0), n(xLeft, zLeft), mat);
			VertexData lr(glm::vec4(xLeft, f(xLeft, zRight), zRight, 1.0), n(xLeft, zRight), mat);
			VertexData rl(glm::vec4(xRight, f(xRight, zLeft), zLeft, 1.0), n(xRight, zLeft), mat);
			VertexData rr(glm::vec4(xRight, f(xRight, zRight), zRight, 1.0), n(xRight, zRight), mat);

			verts.push_back(ll);
			verts.push_back(lr);
			verts.push_back(rr);

			verts.push_back(ll);
			verts.push_back(rr);
			verts.push_back(rl);
		}
	}
	return verts;
}

EShapeData surface = createSurface(silver);

void renderObjects() {
	VertexOps::render(frameBuffer, board, lights, glm::mat4());
	VertexOps::render(frameBuffer, surface, lights, glm::mat4());
}

static void render() {
	frameBuffer.clearColorAndDepthBuffers();
	int width = frameBuffer.getWindowWidth();
	int height = frameBuffer.getWindowHeight();
	VertexOps::viewingTransformation = glm::lookAt(glm::vec3(0, 5, 5), glm::vec3(0, 0, 0), Y_AXIS);
	VertexOps::projectionTransformation = glm::perspective(M_PI_3, 2.0f, 0.5f, 80.0f);
	//VertexOps::projectionTrans = glm::ortho(-4.0f, 4.0f, -2.0f, 2.0f, 0.0f, 10.0f);
	VertexOps::setViewport(0, width - 1, 0, height - 1);
	renderObjects();
	frameBuffer.showColorBuffer();
}

void resize(int width, int height) {
	frameBuffer.setFrameBufferSize(width, height);
	float AR = (float)width / height;

	VertexOps::setViewport(0, width - 1, 0, height - 1);
	VertexOps::projectionTransformation = glm::perspective(M_PI_3, AR, 0.5f, 80.0f);

	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
	const float INC = 0.5;
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
	case 'z':	theLight->lightPosition.z += (isupper(key) ? INC : -INC);
		std::cout << theLight->lightPosition << std::endl;
		break;
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
	GLuint world_Window = glutCreateWindow(extractBaseFilename(__FILE__).c_str());
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	frameBuffer.setClearColor(lightGray);
	glutMouseFunc(mouseUtility);

	glutMainLoop();

	return 0;
}
