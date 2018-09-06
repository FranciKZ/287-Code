#include <iostream>
#include "Defs.h"
#include "Utilities.h"

/**
 * @fn	Window::Window(int W, int H)
 * @brief	Constructs window based on specific size values.
 * @param	W	Width of window.
 * @param	H	Height of window.
 */

Window::Window(int W, int H) {
	width = W;
	height = H;
}

/**
 * @fn	int Window::area() const
 * @brief	Compute the area of the window
 * @return	Area of window.
 */

int Window::area() const {
	return width * height;
}

/**
 * @fn	float Window::aspectRatio() const
 * @brief	Compute aspect ratio of the window.
 * @return	A aspect ratio of window - width/height.
 */

float Window::aspectRatio() const {
	return width / (float)height;
}

/**
 * @fn	BoundingBox3D::BoundingBox3D(float left, float right, float bottom, float top, float back, float front)
 *
 * @brief	Constructor
 * @param	left  	left.
 * @param	right 	right.
 * @param	bottom	bottom.
 * @param	top   	top.
 * @param	back  	back.
 * @param	front 	front.
 */

BoundingBox3D::BoundingBox3D(float left, float right, float bottom, float top, float back, float front) {
		lx = left;
		rx = right;
		ly = bottom;
		ry = top;
		lz = front;
		rz = back;
}

/**
 * @fn	float BoundingBox3D::width() const
 * @brief	Gets the width - x axis
 * @return	Width of bounding box.
 */

float BoundingBox3D::width() const {
	return rx - lx;
}

/**
 * @fn	float BoundingBox3D::height() const
 * @brief	Gets the height - y axis
 * @return	Height of bounding box.
 */

float BoundingBox3D::height() const {
	return ry - ly;
}

/**
 * @fn	float BoundingBox3D::depth() const
 * @brief	Gets the depth - z axis
 * @return	Depth of bounding box.
 */

float BoundingBox3D::depth() const {
	return lz - rz;
}

/**
 * @fn	void Frame::setInverse()
 * @brief	Sets the inverse based on the current parameters.
 * @see Page *** from textbook
 */

void Frame::setInverse() {
	glm::mat4 T;
	T[0][0] = u[0];
	T[0][1] = u[1];
	T[0][2] = u[2];

	T[1][0] = v[0];
	T[1][1] = v[1];
	T[1][2] = v[2];

	T[2][0] = w[0];
	T[2][1] = w[1];
	T[2][2] = w[2];

	T[3][0] = origin[0];
	T[3][1] = origin[1];
	T[3][2] = origin[2];

	inverse = glm::inverse(T);
}

/**
 * @fn	glm::vec3 Frame::toFrameCoords(const glm::vec3 &pt) const
 * @brief	Converts a point to a frame coordinates
 * @param	pt	Point in world coordinates.
 * @return	The frame coordinates of a point given in woord coordinates.
 */

glm::vec3 Frame::toFrameCoords(const glm::vec3 &pt) const {
	return (inverse * glm::vec4(pt.x, pt.y, pt.z, 1.0f)).xyz();
}

/**
* @fn	glm::vec3 Frame::toWorldCoords(const glm::vec3 &pt) const
* @brief	Converts a frame coordinate into the equivalent point in world coordinates
* @param	pt	Point in frame coordinates.
* @return	The world coordinates of a point given in frame coordinates.
*/

glm::vec3 Frame::toWorldCoords(const glm::vec3 &pt) const {
	return origin + pt.x * u + pt.y * v + pt.z * w;
}

/**
 * @fn	glm::vec3 Frame::toFrameVector(const glm::vec3 &V) const
 * @brief	Converts a V (in world system) into equivalent frame vector.
 * @param	V	World vector to process.
 * @return	Frame vector that expresses the same direction as the original.
 */

glm::vec3 Frame::toFrameVector(const glm::vec3 &V) const {
	glm::vec3 A = toFrameCoords(V);
	glm::vec3 B = toFrameCoords(ORIGIN3D);
	return A - B;
}

/**
 * @fn	glm::vec3 Frame::toWorldVector(const glm::vec3 &V) const
 * @brief	Converts a V (in frame system) into equivalent world vector.
 * @param	V	Frame vector to process.
 * @return	World vector that expresses the same direction as the original.
 */

glm::vec3 Frame::toWorldVector(const glm::vec3 &V) const {
	glm::vec3 vectorHead = origin + u * V.x + v * V.y + w * V.z;
	glm::vec3 vectorTail = origin;
	return vectorHead - vectorTail;
}

/**
 * @fn	Frame Frame::createOrthoNormalBasis(const glm::vec3 &pos, const glm::vec3 &w, const glm::vec3 &up)
 * @brief	Creates ortho normal basis given a position and two non-parallel vectors.
 * @param	pos	The position of the new frame's origin.
 * @param	w  	"z" vector in new frame.
 * @param	up 	up vector in new frame.
 * @return	The new ortho normal basis.
 */

Frame Frame::createOrthoNormalBasis(const glm::vec3 &pos, const glm::vec3 &w, const glm::vec3 &up) {
	Frame frame;
	// Fill in
	return frame;
}

/**
* @fn	Frame Frame::createOrthoNormalBasis(const glm::mat4 &viewingMatrix)
* @brief	Creates ortho normal basis given two non-parallel vectors.
* @param	viewingMatrix The viewing matrix created by glm::lookAt
* @return	The equivalent Frame
*/

Frame Frame::createOrthoNormalBasis(const glm::mat4 &viewingMatrix) {
	glm::mat4 vmInverse = glm::inverse(viewingMatrix);
	glm::vec3 u(vmInverse[0]);
	glm::vec3 v(vmInverse[1]);
	glm::vec3 w(vmInverse[2]);
	glm::vec3 eye(vmInverse[3]);
	return Frame(eye, u, v, w);
}

/**
* @fn	glm::mat4 Frame::toViewingMatrix()
* @brief	Returns the viewing matrix equivalent to the frame
* @return	The equivalent viewing matrix
*/

glm::mat4 Frame::toViewingMatrix() const {
	return glm::inverse(glm::mat4(u.x, u.y, u.z, 0,
						v.x, v.y, v.z, 0,
						w.x, w.y, w.z, 0,
						origin.x, origin.y, origin.z, 1));
}

/**
 * @fn	Frame::Frame()
 * @brief	Constructs a new frame equivalent to the world frame.
 */

Frame::Frame() : origin(ORIGIN3D), u(ZEROVEC), v(ZEROVEC), w(ZEROVEC) {
	setInverse();
}

/**
 * @fn	Frame::Frame(const glm::vec3 &O, const glm::vec3 &U, const glm::vec3 &V, const glm::vec3 &W)
 * @brief	Constructs a new frame given 3 orthonormal vectors (assumed to be orthonormal).
 * @param 	O	Origin of new frame.
 * @param	U	New "x" vector.
 * @param 	V	New "y" vector.
 * @param 	W	New "z" vector.
 */

Frame::Frame(const glm::vec3 &O, const glm::vec3 &U, const glm::vec3 &V, const glm::vec3 &W)
	: origin(O), u(U), v(V), w(W) {
	setInverse();
}

/**
 * @fn	void Frame::setFrame(const glm::vec3 &O, const glm::vec3 &U, const glm::vec3 &V, const glm::vec3 &W)
 * @brief	Sets the frame's axes and origin.
 * @param 	O	Origin of new frame.
 * @param	U	New "x" vector.
 * @param 	V	New "y" vector.
 * @param 	W	New "z" vector.
 */

void Frame::setFrame(const glm::vec3 &O, const glm::vec3 &U, const glm::vec3 &V, const glm::vec3 &W) {
	origin = O;
	u = U;
	v = V;
	w = W;
	setInverse();
}

/**
* @fn	ostream &operator << (std::ostream &os, const Frame &frame)
* @brief	Output stream operator for Frame object
* @param 	os	Output stream.
* @param	frame	Frame to stream.
*/

std::ostream &operator << (std::ostream &os, const Frame &frame) {
	os << "Pos: " << frame.origin << std::endl;
	os << "U: " << frame.u << std::endl;
	os << "V: " << frame.v << std::endl;
	os << "W: " << frame.w << std::endl;
	return os;
}
