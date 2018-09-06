#include "Utilities.h"
#include "FrameBuffer.h"

/**
 * @fn	FrameBuffer::FrameBuffer(const int width, const int height)
 * @brief	Constructor
 * @param	width 	The width.
 * @param	height	The height.
 */

FrameBuffer::FrameBuffer(const int width, const int height) : window(width, height) {
	setFrameBufferSize(width, height);
}

/**
 * @fn	FrameBuffer::~FrameBuffer()
 * @brief	Destructor
 */

FrameBuffer::~FrameBuffer() {
	delete[] colorBuffer;
	delete[] depthBuffer;
}

/**
 * @fn	void FrameBuffer::setFrameBufferSize(int width, int height)
 * @brief	Sets frame buffer size
 * @param	width 	The width.
 * @param	height	The height.
 * @see https://www.opengl.org/archives/resources/features/KilgardTechniques/oglpitfall/
 */

void FrameBuffer::setFrameBufferSize(int width, int height) {
	window = Window(width, height);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	delete [] colorBuffer;
	delete [] depthBuffer;

	colorBuffer = new GLubyte[window.area() * BYTES_PER_PIXEL];
	depthBuffer = new float[window.area()];
}

/**
 * @fn	void FrameBuffer::setClearColor(const color &clear)
 * @brief	Sets clear color.
 * @param	clear	Clear color.
 */

void FrameBuffer::setClearColor(const color &clear) {
	clearColorUB[0] = (GLubyte)(clear.r * 255.0f);
	clearColorUB[1] = (GLubyte)(clear.g * 255.0f);
	clearColorUB[2] = (GLubyte)(clear.b * 255.0f);
}

/**
 * @fn	void FrameBuffer::clearColorAndDepthBuffers()
 * @brief	Clears the color and depth buffers
 */

void FrameBuffer::clearColorAndDepthBuffers() {
	for (int y = 0; y < window.height; ++y) {
		for (int x = 0; x < window.width; ++x) {
			std::memcpy(colorBuffer + BYTES_PER_PIXEL * (x + y * window.width),
				clearColorUB, BYTES_PER_PIXEL);
		}
	}
	const int SZ = window.area();
	std::fill(depthBuffer, depthBuffer + SZ, 1.0f);
}

/**
 * @fn	void FrameBuffer::showColorBuffer() const
 * @brief	Shows the contents of the color buffer to screen.
 */

void FrameBuffer::showColorBuffer() const {
	glRasterPos2d(-1, -1);
	glDrawPixels(window.width, window.height, GL_RGB, GL_UNSIGNED_BYTE, colorBuffer);
	glFlush();
}

/**
 * @fn	void FrameBuffer::setColor(int x, int y, const color &rgb)
 * @brief	Sets a color at (x, y)
 * @param	x  	The x coordinate.
 * @param	y  	The y coordinate.
 * @param	rgb	The new RGB value.
 */

void FrameBuffer::setColor(int x, int y, const color &rgb) {
	if (x < 0 || x >= window.width || y < 0 || y >= window.height) {
		return;
	}

	color clampedColor = glm::clamp(rgb, 0.0f, 1.0f);

	GLubyte c[] = { (GLubyte)(clampedColor.r * 255),
					(GLubyte)(clampedColor.g * 255),
					(GLubyte)(clampedColor.b * 255) };

	std::memcpy(colorBuffer + BYTES_PER_PIXEL * (x + y * window.width), c, BYTES_PER_PIXEL);
}

/**
 * @fn	color FrameBuffer::getColor(int x, int y) const
 * @brief	Gets the color at (x, y)
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @return	The color at (x, y)
 */

color FrameBuffer::getColor(int x, int y) const {
	float red, green, blue;

	if (checkInWindow(x, y)) {
		GLubyte c[BYTES_PER_PIXEL];

		// Retrieve color values from the color buffer
		std::memcpy(c, colorBuffer + BYTES_PER_PIXEL * (x + y * window.width), BYTES_PER_PIXEL);

		// Convert individual color components back to floating point values
		red = c[0] / 255.0f;
		green = c[1] / 255.0f;
		blue = c[2] / 255.0f;
	} else {
		exit(0);
		red = clearColorUB[0] / 255.0f;
		green = clearColorUB[1] / 255.0f;
		blue = clearColorUB[2] / 255.0f;
	}
	return color(red, green, blue);
}

/**
* @fn	void FrameBuffer::setDepth(int x, int y, float depth)
* @brief	Sets a depth at (x, y)
* @param	x	 	The x coordinate.
* @param	y	 	The y coordinate.
* @param	depth	The new depth.
*/

void FrameBuffer::setDepth(float x, float y, float depth) {
	setDepth((int)(x), (int)(y), depth);
}

/**
 * @fn	void FrameBuffer::setDepth(int x, int y, float depth)
 * @brief	Sets a depth at (x, y)
 * @param	x	 	The x coordinate.
 * @param	y	 	The y coordinate.
 * @param	depth	The new depth.
 */

void FrameBuffer::setDepth(int x, int y, float depth) {
	if (checkInWindow(x, y)) {
		depthBuffer[y * window.width + x] = depth;
	}
}

/**
* @fn	float FrameBuffer::getDepth(float x, float y) const
* @brief	Gets a depth at (x, y)
* @param	x	The x coordinate.
* @param	y	The y coordinate.
* @return	The depth at (x, y).
*/

float FrameBuffer::getDepth(int x, int y) const {
	if (checkInWindow(x, y)) {
		return depthBuffer[y * window.width + x];
	} else {
		return 0.0f;
	}
}

/**
 * @fn	float FrameBuffer::getDepth(float x, float y) const
 * @brief	Gets a depth at (x, y)
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @return	The depth at (x, y).
 */

float FrameBuffer::getDepth(float x, float y) const {
	return getDepth((int)(x), (int)(y));
}

/**
 * @fn	bool FrameBuffer::checkInWindow(int x, int y) const
 * @brief	Returns true iff (x, y) is a valid window coordinate.
 * @param	x	The x coordinate.
 * @param	y	The y coordinate.
 * @return	True iff (x, y) is a valid window coordinate.
 */

bool FrameBuffer::checkInWindow(int x, int y) const {
	return 0 <= x && x < window.width &&
			0 <= y && y < window.height;
}

/**
 * @fn	void FrameBuffer::setPixel(int x, int y, const color &C, float depth)
 * @brief	Sets a pixel's color and depth values
 * @param	x	 	Window x coordinate.
 * @param	y	 	Window y coordinate.
 * @param	C	 	The color to set.
 * @param	depth	The depth to set
 */

void FrameBuffer::setPixel(int x, int y, const color &C, float depth) {
	setDepth(x, y, depth);
	setColor(x, y, C);
}
