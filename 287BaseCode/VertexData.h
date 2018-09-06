#pragma once
#include "Defs.h"
#include "ColorAndMaterials.h"

/**
 * @struct	VertexData
 * @brief	A vertex data.
 */

struct VertexData {
	glm::vec4 position;		//!< Processed coordinate.
	glm::vec3 normal;		//!< transformed normal vector.
	glm::vec3 worldPosition;//!< Saved world position, for lighting calculations.
	Material material;		//!< This vertex's material.

	VertexData(const glm::vec4 &pos = ORIGIN3DHOMO,
				const glm::vec3 &norm = glm::vec3(0.0, 0.0, 1.0),
				const Material &mat = bronze,
				const glm::vec3 &worldPos = ORIGIN3D);
	VertexData(float w1, const VertexData &vd1,
				float w2, const VertexData &vd2);
	static void addTriVertsAndComputeNormal(std::vector<VertexData> &verts,
		const glm::vec4 &V1, const glm::vec4 &V2,
		const glm::vec4 &V3, const glm::vec3 &n,
		const Material &mat);
	static void addTriVertsAndComputeNormal(std::vector<VertexData> &verts,
		const glm::vec4 &V1, const glm::vec4 &V2, const glm::vec4 &V3,
		const Material &mat);
	static void addTriVertsAndComputeNormals(std::vector<VertexData> &verts,
		const std::vector<glm::vec4> &pts,
		const Material &mat);
	static void addConvexPolyVertsAndComputeNormals(std::vector<VertexData> &verts,
		const std::vector<glm::vec4> &polyCorners,
		const Material &mat);
	static void addConvexPolyVertsAndComputeNormals(std::vector<VertexData> &verts,
		const glm::vec4 &p1, const glm::vec4 &p2,
		const glm::vec4 &p3, const glm::vec4 &p4,
		const Material &mat);
	VertexData operator + (const VertexData &other) const;
};

VertexData operator * (float w, const VertexData &V1);
