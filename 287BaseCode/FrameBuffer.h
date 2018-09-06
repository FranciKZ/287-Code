#pragma once

#include "defs.h"
#include "ColorAndMaterials.h"

const int BYTES_PER_PIXEL = 3;			//!< RGB requires 3 bytes.

/**
 * @struct	FrameBuffer
 * @brief	Represents a framebuffer. Two identically sized 2D arrays. The color
 * 			buffer stores the colors and the depth buffer stores the corresponding
 * 			depth at each pixel.
 */

struct FrameBuffer {
	FrameBuffer(const int width, const int height);
	~FrameBuffer();
	void setFrameBufferSize(int width, int height);
	void setClearColor(const color &clearColor);
	void setColor(int x, int y, const color &C);
	color getColor(int x, int y) const;

	void clearColorAndDepthBuffers();
	void showColorBuffer() const;
	int getWindowWidth() const { return window.width; }
	int getWindowHeight() const { return window.height; }

	void setDepth(float x, float y, float depth);
	void setDepth(int x, int y, float depth);
	float getDepth(int x, int y) const;
	float getDepth(float x, float y) const;

	void setPixel(int x, int y, const color &C, float depth);
protected:
	bool checkInWindow(int x, int y) const;
	Window window;							//!< Dimensions of framebuffer
	GLubyte clearColorUB[BYTES_PER_PIXEL];	//!< Clear color
	GLubyte *colorBuffer;					//!< 2D array for holding colors
	float *depthBuffer;						//!< 2D array for holding depths
};
