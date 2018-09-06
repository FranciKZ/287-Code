#include "VertexData.h"
#include "Utilities.h"

/**
 * @fn	VertexData::VertexData(const glm::vec4 &pos, const glm::vec3 &norm, const Material &mat, const glm::vec3 &worldPos) : position(pos), normal(glm::normalize(norm)), material(mat), worldPosition(worldPos)
 * @brief	Constructor
 * @param	pos			Current coordinate.
 * @param	norm		Normal vector
 * @param	mat			Material
 * @param	worldPos	World position.
 */

VertexData::VertexData(const glm::vec4 &pos,
				const glm::vec3 &norm,
				const Material &mat,
				const glm::vec3 &worldPos) :
	position(pos), normal(glm::normalize(norm)), material(mat), worldPosition(worldPos) {
}

/**
 * @fn	VertexData::VertexData(float w1, const VertexData &vd1, float w2, const VertexData &vd2)
 * @brief	Constructs object using weighted average of two VertexData objects.
 * @param	w1 	Weight #1.
 * @param	vd1	VertexData #1.
 * @param	w2 	Weight #2.
 * @param	vd2	VertexData #2.
 */

VertexData::VertexData(float w1, const VertexData &vd1,
					float w2, const VertexData &vd2)
					: position(weightedAverage(w1, vd1.position, w2, vd2.position)),
						normal(weightedAverage(w1, vd1.normal, w2, vd2.normal)),
						material(weightedAverage(w1, vd1.material, w2, vd2.material)),
						worldPosition(weightedAverage(w1, vd1.worldPosition, w2, vd2.worldPosition)) {
}

/**
 * @fn	void VertexData::addTriVertsAndComputeNormal(std::vector<VertexData> &verts, const glm::vec4 &V1, const glm::vec4 &V2, const glm::vec4 &V3, const glm::vec3 &n, const Material &mat)
 * @brief	Adds a triangle vertices, adding vertices to end of verts. Vertices are specified
 * 			in counterclockwise order.
 * @param [in,out]	verts	The vector of vertices.
 * @param 		  	V1   	The first vertice
 * @param 		  	V2   	The second vertice.
 * @param 		  	V3   	The third vertice.
 * @param 		  	n	 	Normal vector.
 * @param 		  	mat  	The matrix.
 */

void VertexData::addTriVertsAndComputeNormal(std::vector<VertexData> &verts,
											const glm::vec4 &V1,
											const glm::vec4 &V2,
											const glm::vec4 &V3,
											const glm::vec3 &n,
											const Material &mat) {
	verts.push_back(VertexData(V1, n, mat));
	verts.push_back(VertexData(V2, n, mat));
	verts.push_back(VertexData(V3, n, mat));
}

/**
 * @fn	void VertexData::addTriVertsAndComputeNormal(std::vector<VertexData> &verts, const glm::vec4 &V1, const glm::vec4 &V2, const glm::vec4 &V3, const Material &mat)
 * @brief	Adds a triangle vertices and computes normal, adding vertices to end of verts. Vertices are specified
 * 			in counterclockwise order.
 * @param [in,out]	verts	The vector of vertices.
 * @param 		  	V1   	The first vertice
 * @param 		  	V2   	The second vertice.
 * @param 		  	V3   	The third vertice.
 * @param 		  	mat  	Material.
 */

void VertexData::addTriVertsAndComputeNormal(std::vector<VertexData> &verts,
											const glm::vec4 &V1,
											const glm::vec4 &V2,
											const glm::vec4 &V3,
											const Material &mat) {
	glm::vec3 n = normalFrom3Points(V1.xyz, V2.xyz, V3.xyz);
	verts.push_back(VertexData(V1, n, mat));
	verts.push_back(VertexData(V2, n, mat));
	verts.push_back(VertexData(V3, n, mat));
}

/**
 * @fn	void VertexData::addTriVertsAndComputeNormals(std::vector<VertexData> &verts, const std::vector<glm::vec4> &pts, const Material &mat)
 * @brief	Adds multiple triangles to triangle vertices
 * @param [in,out]	verts	The vector of vertices.
 * @param 		  	pts  	The new triangle vertices to be added.
 * @param 		  	mat  	Material.
 */

void VertexData::addTriVertsAndComputeNormals(std::vector<VertexData> &verts,
								const std::vector<glm::vec4> &pts,
								const Material &mat) {
	unsigned int numTris = pts.size() / 3;
	for (unsigned int i = 0; i < numTris; i++) {
		const glm::vec4 &V1 = pts[3 * i];
		const glm::vec4 &V2 = pts[3 * i + 1];
		const glm::vec4 &V3 = pts[3 * i + 2];
		addTriVertsAndComputeNormal(verts, V1, V2, V3, mat);
	}
}

/**
 * @fn	void VertexData::addConvexPolyVertsAndComputeNormals(std::vector<VertexData> &verts, const std::vector<glm::vec4> &polyCorners, const Material &mat)
 * @brief	Converts polygon into triangles vertices and adds them to vert.
 * @param [in,out]	verts	The vector of vertices.
 * @param 		  	polyCorners  	The polygon to be added.
 * @param 		  	mat  	Material.

 */

void VertexData::addConvexPolyVertsAndComputeNormals(std::vector<VertexData> &verts,
										const std::vector<glm::vec4> &polyCorners,
										const Material &mat) {
	unsigned int numTris = polyCorners.size() - 2;
	for (unsigned int i = 0; i < numTris; i++) {
		const glm::vec4 &V1 = polyCorners[0];
		const glm::vec4 &V2 = polyCorners[i + 1];
		const glm::vec4 &V3 = polyCorners[i + 2];
		addTriVertsAndComputeNormal(verts, V1, V2, V3, mat);
	}
}

/**
 * @fn	void VertexData::addConvexPolyVertsAndComputeNormals(std::vector<VertexData> &verts, const glm::vec4 &p1, const glm::vec4 &p2, const glm::vec4 &p3, const glm::vec4 &p4, const Material &mat)
 * @brief	Adds a convex quadrilateral.
 * @param [in,out]	verts	The vertices.
 * @param 		  	p1   	The first vertex.
 * @param 		  	p2   	The second vertex.
 * @param 		  	p3   	The third vertex.
 * @param 		  	p4   	The fourth vertex.
 * @param 		  	mat  	The matrix.
 */

void VertexData::addConvexPolyVertsAndComputeNormals(std::vector<VertexData> &verts,
										const glm::vec4 &p1,
										const glm::vec4 &p2,
										const glm::vec4 &p3,
										const glm::vec4 &p4,
										const Material &mat) {
	addConvexPolyVertsAndComputeNormals(verts, std::vector<glm::vec4>({ p1, p2, p3, p4 }), mat);
}

/**
 * @fn	VertexData operator* (float w, const VertexData &data)
 * @brief	Multiplication operator for VertexData objects
 * @param	w   	The scalar multiplier.
 * @param	data	Vertex data to scale.
 * @return	The scaled Vertex data.
 */

VertexData operator * (float w, const VertexData &data) {
	VertexData result(w*data.position, w*data.normal, w*data.material, w*data.worldPosition);
	return result;
}

/**
 * @fn	VertexData VertexData::operator+ (const VertexData &other) const
 * @brief	Addition operator for VertexData objects
 * @param	other	The 2nd VertexData object.
 * @return	The raw summation of the two VertexData objects
 */

VertexData VertexData::operator + (const VertexData &other) const {
	VertexData result(*this);
	result.material += other.material;
	result.normal += other.normal;
	result.position += other.position;
	result.worldPosition += other.worldPosition;
	return result;
}
