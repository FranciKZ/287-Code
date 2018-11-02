#include "Utilities.h"
/*
* Answer questions here.
*/

/*
* Creates a mat2 and loads it with the following matrix:
* | 2  1 |
* | 4 -2 |
* Prints that matrix, in addition to it's inverse. From a prior class:
* The inverse is:
* | 0.25   0.125 |
* | 0.50  -0.250 |
*/

void basicTest() {
	std::cout << __FUNCTION__ << std::endl;
	glm::mat2 mat;
	mat[0][0] = 2;
	mat[1][0] = 1;
	mat[0][1] = 4;
	mat[1][1] = -2;

	glm::mat2 inv = glm::inverse(mat);
	std::cout << mat << std::endl;
	std::cout << inv << std::endl;
}

int main(int argc, char *argv[]) {
	glm::vec3 V1(1, 2, 3);
	glm::vec3 V2(-1, -2, -3);

	glm::mat2 M2x2A(2, 4, 1, -2);
	glm::mat2 M2x2B(0, 1, 2, 4);
	glm::mat2 M2x2C(1, 0, 4, 2);

	glm::mat3 M3x3A(1, 0, 3, 2, 1, 1, 3, 2, 0);
	glm::mat3 M3x3B(1, 2, 3, 2, 1, 0, 0, 1, 0);
	glm::mat3 M3x3C(10, 0, 0, 0, 20, 0, 0, 0, 30);

	std::vector<glm::mat3> vMats = { M3x3A, M3x3B, M3x3C };
	std::vector<glm::vec3> vVecs = { V1, V2, V2, V1 };

	std::cout << "V1:" << V1 << std::endl;
	std::cout << "V2:" << V2 << std::endl;

	std::cout << "M2x2A:" << M2x2A << std::endl;
	std::cout << "M3x3A:" << M3x3A << std::endl;
	std::cout << "M3x3B:" << M3x3B << std::endl;
	std::cout << "M3x3C:" << M3x3C << std::endl;

	basicTest();

	std::cout << "Get M3x3A Row 0" << getRow(M3x3A, 0) << std::endl;
	std::cout << "Get M3x3A Row 1" << getRow(M3x3A, 1) << std::endl;
	std::cout << "Get M3x3A Row 2" << getRow(M3x3A, 2) << std::endl;

	std::cout << "Get M3x3A Col 0" << getCol(M3x3A, 0) << std::endl;
	std::cout << "Get M3x3A Col 1" << getCol(M3x3A, 1) << std::endl;
	std::cout << "Get M3x3A Col 2" << getCol(M3x3A, 2) << std::endl;

	std::cout << isInvertible(M2x2A) << std::endl;
	std::cout << isInvertible(M3x3A) << std::endl;

	std::cout << solveLinearSystem(M3x3A, V1) << std::endl;

	std::cout << addMatrices(vMats) << std::endl;
	std::cout << multiplyMatrices(vMats) << std::endl;

	std::cout << multiplyMatrixAndVertex(M3x3A, V1) << std::endl;
	std::cout << multiplyMatricesAndVertex(vMats, V1) << std::endl;
	std::cout << multiplyMatrixAndVertices(M3x3A, vVecs) << std::endl;
	std::cout << multiplyMatricesAndVertices(vMats, vVecs) << std::endl;

	std::cout << glm::inverse(M2x2A) << std::endl;
	std::cout << glm::inverse(M2x2B) << std::endl;
	std::cout << glm::inverse(M2x2C) << std::endl;
	return 0;
}



