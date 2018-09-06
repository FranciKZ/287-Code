#include <ctime>
#include <vector>
#include "defs.h"
#include "Utilities.h"
#include "FrameBuffer.h"
#include "ColorAndMaterials.h"

int main(int argc, char *argv[]) {
	std::cout << "Expected: 45" << " " << "Result: " << directionInDegrees(2, 10, 3, 11) << std::endl;
	std::cout << "Expected: 225" << " " << "Result: " << directionInDegrees(3, 11, 2, 10) << std::endl;
	std::cout << "Expected: 45" << " " << "Result: " << directionInDegrees(0, 0, 10, 10) << std::endl;
	std::cout << "Expected: 270" << " " << "Result: " << directionInDegrees(2, 2, 2, 0) << std::endl;

	std::cout << "Expected: 45" << " " << "Result: " << directionInDegrees(glm::vec2(2, 10), glm::vec2(3, 11)) << std::endl;

	std::cout << "Expected: 0" << " " << "Result: " << directionInDegrees(glm::vec2(1, 0)) << std::endl;
	std::cout << "Expected: 45" << " " << "Result: " << directionInDegrees(glm::vec2(1, 1)) << std::endl;
}
