#pragma once
#include <memory>
#include "Defs.h"

/**
 * @struct	Image
 * @brief	Represents a rectangular RGB image.
 */

struct Image {
	int W, H;
	color *pixels;
	Image(char *ppmFileName);
	~Image() { delete[] pixels; }
	color getPixel(float u, float v) const;
};
