#include <ctime>
#include <iostream>
#include "ColorAndMaterials.h"
#include "FrameBuffer.h"
#include "Utilities.h"
#include "Rasterization.h"

FrameBuffer colorBuffer(500, 500);
const int N = 50;
int windowWidth, windowHeight;

std::vector<glm::vec3> triangleVertices = { glm::vec3(-2 * N,2 * N,1), glm::vec3(-N,2 * N,1), glm::vec3(-1.5*N,3 * N,1) };
std::vector<glm::vec3> square1Vertices = { glm::vec3(-N,-N,1), glm::vec3(N,-N,1),
											glm::vec3(N,N,1), glm::vec3(-N,N,1) };
std::vector<glm::vec3> square2Vertices = { glm::vec3(3 * N,-2 * N,1), glm::vec3(3 * N,-3 * N,1),
											glm::vec3(2 * N,-3 * N,1), glm::vec3(2 * N,-2 * N,1) };

int displayedProblem = 0;

std::vector<glm::vec3> transformVertices(const glm::mat3 &transMatrix, const std::vector<glm::vec3> &vertices) {
	std::vector<glm::vec3> transformedVertices;

	for (size_t i = 0; i < vertices.size(); i++) {
		glm::vec3 vt(transMatrix * vertices[i]);
		transformedVertices.push_back(vt);
	}

	return transformedVertices;
}
void drawWirePolygonWithShift(std::vector<glm::vec3> verts, const color &C) {
	int W2 = colorBuffer.getWindowWidth()/2;
	int H2 = colorBuffer.getWindowHeight()/2;
	for (unsigned int i = 0; i < verts.size(); i++) {
		verts[i].x += W2;
		verts[i].y += H2;
	}
	drawWirePolygon(colorBuffer, verts, C);
}
void drawOne(const glm::mat3 &TM, const std::vector<glm::vec3> &verts, bool drawAxis = true) {
	std::vector<glm::vec3> vertsTransformed = transformVertices(TM, verts);
	drawWirePolygonWithShift(verts, black);
	drawWirePolygonWithShift(vertsTransformed, red);
	if (drawAxis) {
		drawAxisOnWindow(colorBuffer);
	}
}
void drawAll(const glm::mat3 &TM) {
	drawOne(TM, triangleVertices, false);
	drawOne(TM, square1Vertices, false);
	drawOne(TM, square2Vertices, false);
	drawAxisOnWindow(colorBuffer);
}

// Draw all the shapes, transformed by S(2, 0.5)
void doScaleBy2xOneHalf() {
	drawAll(S(2, 0.5));
}

// Draw all the shapes, transformed by T(50, 50)
void doTranslate50_50() {
	drawAll(T(50, 50));
}

// Draw all the shapes, transformed by R(45)
void doRotate45() {
	drawAll(R(45));
}

// Draw all the shapes, transformed by R(-10)
void doRotateNeg10() {
	drawAll(R(-10));
}

// Draw all shapes, reflected across the Y axis
void doReflectAcrossYaxis() {
	extern glm::mat3 reflectAcrossYaxis();
	glm::mat3 TM = reflectAcrossYaxis();
	drawAll(TM);
}

// Draw all shapes, reflected across the origin
void doReflectAcrossOrigin() {
	extern glm::mat3 reflectAcrossOrigin();
	glm::mat3 TM = reflectAcrossOrigin();
	drawAll(TM);
}

// Draw only triangle, scaled 2X about its center (-1.5N, 2.5N)
void doScale2XAboutCenterOfTriangle() {
	extern glm::mat3 scale2XAboutPoint(float x, float y);
	glm::mat3 TM;// = scale2XAboutPoint(-1.5*N, 2.5*N);
	drawOne(TM, triangleVertices);
}

// Draw all shapes, reflected across y=x+50
void doReflectAcrossLineYeqXplus50() {
	extern glm::mat3 reflectAcrossLineYeqXplus50();
	glm::mat3 TM;// = reflectAcrossLineYeqXplus50();
	drawAll(TM);
}

// Animate the rotation of the square1
void doAnimationOfRotationByAngle() {
	static double angle = 0.0;
	angle += 1.0;
	drawOne(R(angle), square1Vertices);
}

// Horizontally shear square 1 by factor 0.5
void doHorzShearOneHalf() {
	drawOne(horzShear(0.5), square1Vertices);
}

// Vertically shear square 1 by factor 0.5
void doVertShearOneHalf() {
	drawOne(vertShear(0.5), square1Vertices);
}

// Render square1 so that it rotates about its own axis, and then orbits the origin.
void doSquareRotatingAroundOwnAxisAndAroundSun() {
	static double D = 0;
	D += glm::radians(45.0f);
 	extern glm::mat3 rotateAroundOwnAxisAndAroundOrigin(float D, float ang1, float ang2);
	glm::mat3 TM = rotateAroundOwnAxisAndAroundOrigin(4.0 * N, 4 * D, D);
	std::vector<glm::vec3> square1VerticesTransformed = transformVertices(TM, square1Vertices);
	drawWirePolygonWithShift(square1VerticesTransformed, red);
	drawAxisOnWindow(colorBuffer);
}

typedef void(*TRANS)();

struct DisplayFunc {
	TRANS func;
	std::string name;
	DisplayFunc(TRANS t, std::string n) : func(t), name(n) {}
};
std::vector<DisplayFunc> funcs = { DisplayFunc(doScaleBy2xOneHalf, "Scale by 2 and 1/2"),
									DisplayFunc(doTranslate50_50, "Trans 50 50"),
									DisplayFunc(doRotate45, "Rotate 45"),
									DisplayFunc(doRotateNeg10, "Rotate -10"),
									DisplayFunc(doReflectAcrossYaxis, "Reflect across Y"),
									DisplayFunc(doReflectAcrossOrigin, "Reflect across origin"),
									DisplayFunc(doScale2XAboutCenterOfTriangle, "Scale 2X about point"),
									DisplayFunc(doReflectAcrossLineYeqXplus50, "Reflect across y=x+50"),
									DisplayFunc(doAnimationOfRotationByAngle, "Rotate by angle"),
									DisplayFunc(doHorzShearOneHalf, "Horizontal shear"),
									DisplayFunc(doVertShearOneHalf, "Vertical shear"),
									DisplayFunc(doSquareRotatingAroundOwnAxisAndAroundSun, "Orbiting planet"),
};

void render() {
	colorBuffer.clearColorAndDepthBuffers();
	(*funcs[displayedProblem].func)();				// call the correct function
	colorBuffer.showColorBuffer();
}

void resize(int width, int height) {
	colorBuffer.setFrameBufferSize(width, height);
	windowWidth = width;
	windowHeight = height;
	glutPostRedisplay();
}

void timer(int id) {
	glutTimerFunc(TIME_INTERVAL, timer, 0);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	const double INC = 0.5;
	switch (key) {
	case ESCAPE:
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void problemMenu(int value) {
	if (value < (int)funcs.size()) {
		displayedProblem = value;
		glutSetWindowTitle(funcs[displayedProblem].name.c_str());
	} else {
		glutLeaveMainLoop();
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(500, 500);
	GLuint world_Window = glutCreateWindow(__FILE__);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(TIME_INTERVAL, timer, 0);
	glutMouseFunc(mouseUtility);

	int menu1id = glutCreateMenu(problemMenu);
	for (unsigned int i = 0; i < funcs.size(); i++) {
		glutAddMenuEntry(funcs[i].name.c_str(), i);
	}
	glutAddMenuEntry("Quit", (int)funcs.size());
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	colorBuffer.setClearColor(white);

	glutMainLoop();

	return 0;
}