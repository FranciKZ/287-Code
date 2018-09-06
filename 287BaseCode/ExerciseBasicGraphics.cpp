#include <ctime>
#include <vector>
#include "defs.h"
#include "Utilities.h"
#include "FrameBuffer.h"
#include "ColorAndMaterials.h"
#include "Rasterization.h"

FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

void closed5x5Square(int x, int y, color C) {
	drawLine(frameBuffer, x - 2, y - 2, x + 2, y - 2, C);
	drawLine(frameBuffer, x - 2, y - 1, x + 2, y - 1, C);
	drawLine(frameBuffer, x - 2,     y, x + 2,     y, C);
	drawLine(frameBuffer, x - 2, y + 1, x + 2, y + 1, C);
	drawLine(frameBuffer, x - 2, y + 2, x + 2, y + 2, C);
}

void closed5x5Square(const glm::vec2 &centerPt, color C) {
	closed5x5Square(centerPt.x, centerPt.y, C);
}

void open5x5Square(const glm::vec2 &centerPt, color C) {
	drawLine(frameBuffer, centerPt.x - 2, centerPt.y + 2, centerPt.x + 2, centerPt.y + 2, C);
	drawLine(frameBuffer, centerPt.x - 2, centerPt.y - 2, centerPt.x + 2, centerPt.y - 2, C);

	drawLine(frameBuffer, centerPt.x - 2, centerPt.y + 2, centerPt.x - 2, centerPt.y - 2, C);
	drawLine(frameBuffer, centerPt.x + 2, centerPt.y + 2, centerPt.x + 2, centerPt.y - 2, C);
}
// length of small arc = perc * 2pi
// length of arc arc = 2pi - perc * 2pi
// rad is radius
void pieChart(const glm::vec2 &centerPt, float rad, float perc, const color &C1, const color &C2) {
	float smallPerc = perc * M_2PI;
	float bigPerc = M_2PI - smallPerc;
	float startPoint = smallPerc / 2;
	drawArc(frameBuffer, centerPt, rad, startPoint, bigPerc, C1);
	glm::vec2 bigStartPoint(pointOnCircle(centerPt, rad, startPoint));
	glm::vec2 bigEndPoint(pointOnCircle(centerPt, rad, (startPoint + bigPerc)));
	drawLine(frameBuffer, centerPt.x, centerPt.y, bigStartPoint.x, bigStartPoint.y, C1);
	drawLine(frameBuffer, centerPt.x, centerPt.y, bigEndPoint.x, bigEndPoint.y, C1);

	glm::vec2 newCenter(centerPt.x + 20, centerPt.y);
	glm::vec2 smallStartPoint(pointOnCircle(newCenter, rad, startPoint + bigPerc));
	glm::vec2 smallEndPoint(pointOnCircle(newCenter, rad, startPoint));
	drawArc(frameBuffer, newCenter, rad, (startPoint + bigPerc), smallPerc, C2);
	drawLine(frameBuffer, newCenter.x, newCenter.y, smallStartPoint.x, smallStartPoint.y, C2);
	drawLine(frameBuffer, newCenter.x, newCenter.y, smallEndPoint.x, smallEndPoint.y, C2);
}

void render() {
	frameBuffer.clearColorAndDepthBuffers();
	closed5x5Square(50, 50, red);
	closed5x5Square(glm::vec2(100, 50), green);
	open5x5Square(glm::vec2(150, 50), blue);
	pieChart(glm::vec2(250, 100), 50, 0.15, red, green);
	frameBuffer.showColorBuffer();
}

void resize(int width, int height) {
	frameBuffer.setFrameBufferSize(width, height);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case ESCAPE:	glutLeaveMainLoop();
					break;
	}
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

	frameBuffer.setClearColor(black);

	glutMainLoop();

	return 0;
}