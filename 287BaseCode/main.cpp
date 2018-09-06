#include <ctime>
#include <vector>
#include "defs.h"
#include "Utilities.h"
#include "FrameBuffer.h"
#include "ColorAndMaterials.h"

int main(int argc, char *argv[]) {
	std::cout << glm::cos(0.0f) << std::endl;
	std::cout << std::cos(0.0f) << std::endl;

	std::cout << radians(90.0f) << std::endl;

	std::cout << "Hello World" << std::endl;
	float a = 10;
	float b = 10;
	swap(a, b);
	return 0;
}
