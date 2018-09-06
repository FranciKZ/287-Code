#include "EShape.h"

/**
 * @fn	EShapeData EShape::createEDisk(const Material &mat, float radius, int slices)
 * @brief	Creates a disk.
 * @param	mat   	Material.
 * @param	radius	Radius.
 * @param	slices	Number of slices.
 * @return	The new disk.
 */

EShapeData EShape::createEDisk(const Material &mat, float radius, int slices) {
	EShapeData result;
	return result;
}

/**
 * @fn	EShapeData EShape::createEPyramid(const Material &mat, float width, float height)
 * @brief	Creates a pyramid with origin at (0,0,0) and pointing toward +y.
 * @param	mat   	Material.
 * @param	width 	Width.
 * @param	height	Height.
 * @return	The new pyramid.
 */

EShapeData EShape::createEPyramid(const Material &mat, float width, float height) {
	EShapeData verts;
	return verts;
}

/**
 * @fn	EShapeData EShape::createECylinder(const Material &mat, float radius, float height, int slices)
 * @brief	Creates cylinder, which is centered on (0,0,0) and aligned with y axis.
 * @param	mat   	Material.
 * @param	radius	Radius.
 * @param	height	Height.
 * @param	slices	Slices.
 * @param	stacks	Stacks.
 * @return	The new cylinder.
 */

EShapeData EShape::createECylinder(const Material &mat, float R, float height, int slices, int stacks) {
	EShapeData result;
	return result;
}

/**
 * @fn	EShapeData EShape::createECone(const Material &mat, float radius, float height, int slices, int stacks)
 * @brief	Creates cone, which is aligned with y axis.
 * @param	mat   	Material.
 * @param	radius	Radius.
 * @param	height	Height.
 * @param	slices	Slices.
 * @param	stacks	Stacks.
 * @return	The new cone.
 */

EShapeData EShape::createECone(const Material &mat, float R, float height, int slices, int stacks) {
	EShapeData result;
	return result;
}

/**
 * @fn	EShapeData EShape::createECube(const Material &mat, float width, float height, float depth)
 * @brief	Creates cube
 * @param	mat   	Material.
 * @param	width 	Width.
 * @param	height	Height.
 * @param	depth 	Depth.
 * @return	The new cube.
 */

EShapeData EShape::createECube(const Material &mat, float width, float height, float depth) {
	EShapeData result;
	return result;
}

/**
 * @fn	EShapeData EShape::createETriangles(const Material &mat, const std::vector<glm::vec4> &V)
 * @brief	Creates triangles
 * @param	mat	Material.
 * @param	V  	Vector of objects to process.
 * @return	The new triangles.
 */

EShapeData EShape::createETriangles(const Material &mat, const std::vector<glm::vec4> &V) {
	EShapeData result;
	size_t numTris = V.size() / 3;
	for (unsigned int i = 0; i < numTris; i++) {
		int start = 3 * i;
		const glm::vec4 &A = V[start];
		const glm::vec4 &B = V[start + 1];
		const glm::vec4 &C = V[start + 2];
		VertexData::addTriVertsAndComputeNormal(result, A, B, C, mat);
	}
	return result;
}

/**
 * @fn	EShapeData EShape::createEPlanes(const Material &mat, const std::vector<glm::vec4> &V)
 * @brief	Creates planes, which are defined by 4 corners.
 * @param	mat	Material.
 * @param	V  	Vector of points to process.
 * @return	The new planes.
 */

EShapeData EShape::createEPlanes(const Material &mat, const std::vector<glm::vec4> &V) {
	EShapeData result;
	size_t numPlanes = V.size() / 4;
	for (unsigned int i = 0; i < numPlanes; i++) {
		int start = 4 * i;
		const glm::vec4 &A = V[start];
		const glm::vec4 &B = V[start + 1];
		const glm::vec4 &C = V[start + 2];
		const glm::vec4 &D = V[start + 3];
		VertexData::addConvexPolyVertsAndComputeNormals(result, A, B, C, D, mat);
	}
	return result;
}

/**
 * @fn	EShapeData EShape::createELines(const Material &mat, const std::vector<glm::vec4> &V)
 * @brief	Creates lines
 * @param	mat	Material.
 * @param	V  	A vector of point-pairs.
 * @return	The new lines.
 */

EShapeData EShape::createELines(const Material &mat, const std::vector<glm::vec4> &V) {
	EShapeData result;
	for (unsigned int i = 0; i < V.size(); i++) {
		result.push_back(VertexData(V[i], ZEROVEC, mat));
	}
	return result;
}

/**
 * @fn	EShapeData EShape::createECheckerBoard(const Material &mat1, const Material &mat2, float WIDTH, float HEIGHT, int DIV)
 * @brief	Creates checker board pattern.
 * @param	mat1  	Material #1.
 * @param	mat2  	Material #2.
 * @param	WIDTH 	Width of overall plane.
 * @param	HEIGHT	Height of overall plane.
 * @param	DIV   	Number of divisions.
 * @return	The vertices in the checker board.
 */

EShapeData EShape::createECheckerBoard(const Material &mat1, const Material &mat2, float WIDTH, float HEIGHT, int DIV) {
	EShapeData result;

	const float INC = (float)WIDTH / DIV;
	for (int X = 0; X < DIV; X++) {
		bool isMat1 = X % 2 == 0;
		for (float Z = 0; Z < DIV; Z++) {
			glm::vec4 V0(-WIDTH / 2.0f + X*INC, 0.0f, -WIDTH / 2 + Z*INC, 1.0f);
			glm::vec4 V1 = V0 + glm::vec4(0.0f, 0.0f, INC, 0.0f);
			glm::vec4 V2 = V0 + glm::vec4(INC, 0.0f, INC, 0.0f);
			glm::vec4 V3 = V0 + glm::vec4(INC, 0.0f, 0.0f, 0.0f);
			const Material &mat = isMat1 ? mat1 : mat2;

			result.push_back(VertexData(V0, Y_AXIS, mat));
			result.push_back(VertexData(V1, Y_AXIS, mat));
			result.push_back(VertexData(V2, Y_AXIS, mat));

			result.push_back(VertexData(V2, Y_AXIS, mat));
			result.push_back(VertexData(V3, Y_AXIS, mat));
			result.push_back(VertexData(V0, Y_AXIS, mat));
			isMat1 = !isMat1;
		}
	}
	return result;
}

/**
 * @fn	std::vector<VertexData> createSidePanel(const Material &mat, const glm::vec2 &V1, const glm::vec2 &V2)
 * @brief	Creates side panel for an extrusion.
 * @param	mat	Material.
 * @param	V1 	The first vertex of the side panel.
 * @param	V2 	The second vertex of the side panel.
 * @return	The vertices in the new side panel.
 */

static std::vector<VertexData> createSidePanel(const Material &mat, const glm::vec2 &V1, const glm::vec2 &V2) {
	std::vector<VertexData> verts;
	return verts;
}
