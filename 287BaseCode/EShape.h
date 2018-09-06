#pragma once
#pragma once

#include <utility>
#include "VertexData.h"
#include "FrameBuffer.h"
#include "Light.h"

typedef std::vector<VertexData> EShapeData;

/**
 * @struct	EShape
 * @brief	This class contains functions that create explicitly represented shapes.
 * 			This class is used within pipeline applications. The objects returned by
 * 			these routines are vectors of VertexData, where each successive triplet
 * 			is a triangle.
 */

struct EShape {
	static EShapeData createEPyramid(const Material &mat, float width = 1.0f, float height = 1.0f);
	static EShapeData createECylinder(const Material &mat, float radius = 1.0f, float height = 1.0, int slices = SLICES, int stacks = SLICES);
	static EShapeData createEDisk(const Material &mat, float radius = 1.0f, int slices = SLICES);
	static EShapeData createECone(const Material &mat, float radius = 1.0f, float height = 1.0, int slices = SLICES, int stacks = SLICES);
	static EShapeData createECube(const Material &mat, float width = 1.0f, float height = 1.0f, float depth = 1.0f);
	static EShapeData createETriangles(const Material &mat, const std::vector<glm::vec4> &pts);
	static EShapeData createEPlanes(const Material &mat, const std::vector<glm::vec4> &corners);
	static EShapeData createELines(const Material &mat, const std::vector<glm::vec4> &corners);
	static EShapeData createECheckerBoard(const Material &mat1, const Material &mat2, float WIDTH, float HEIGHT, int DIV);
	static EShapeData createExtrusion(const Material &mat, const std::vector<glm::vec2> &V);
};
