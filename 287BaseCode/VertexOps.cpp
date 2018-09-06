#include "VertexOps.h"

// Pipeline transformation matrices
glm::mat4 VertexOps::modelingTransformation;
glm::mat4 VertexOps::viewingTransformation;
glm::mat4 VertexOps::projectionTransformation;
glm::mat4 VertexOps::viewportTransformation;
bool VertexOps::renderBackFaces = true;

const BoundingBox3D VertexOps::ndc(-1, 1, -1, 1, -1, 1);	//l,r,b,t,n,f
BoundingBoxi VertexOps::viewport(0, WINDOW_WIDTH - 1, 0, WINDOW_HEIGHT - 1);

// Planes describing the normalized device coordinates view volume - 2x2x2 cube
std::vector<IPlane> VertexOps::ndcPlanes{ IPlane(glm::vec3(0, 1, 0), glm::vec3(0, -1, 0)),
											IPlane(glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0)),
											IPlane(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1)),
											IPlane(glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)),
											IPlane(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0)),
											IPlane(glm::vec3(0, 0, -1), glm::vec3(0, 0, 1)) };

/**
 * @fn	std::vector<VertexData> triangulate(const std::vector<VertexData> &poly)
 * @brief	Triangulates the given polygon
 * @param	poly	The polygon to be decomposed into individual triangles.
 * @return	A vector of triangles, which comprise the original polygon.
 */

std::vector<VertexData> triangulate(const std::vector<VertexData> &poly) {
	std::vector<VertexData> triangles;

	for (unsigned int i = 1; i < poly.size() - 1; i++) {
		triangles.push_back(poly[0]);
		triangles.push_back(poly[i]);
		triangles.push_back(poly[i + 1]);
	}

	return triangles;
}

/**
 * @fn	std::vector<VertexData> VertexOps::clipAgainstPlane(std::vector<VertexData> &verts, const IPlane &plane)
 * @brief	Clips a polygon against a single plane
 * @param [in,out]	verts	The array of vertices.
 * @param 		  	plane	The plane that will do the clipping.
 * @return	The polygon that exludes the portions outside the given plane.
 */

std::vector<VertexData> VertexOps::clipAgainstPlane(std::vector<VertexData> &verts, const IPlane &plane) {
	std::vector<VertexData> output;

	if (verts.size() > 2) {
		verts.push_back(verts[0]);

		for (unsigned int i = 1; i < verts.size(); i++) {
			bool v0In = plane.insidePlane(verts[i - 1].position.xyz);
			bool v1In = plane.insidePlane(verts[i].position.xyz);

			if (v0In && v1In) {
				output.push_back(verts[i]);
			} else if (v0In || v1In) {
				float t;
				plane.findIntersection(verts[i - 1].position.xyz, verts[i].position.xyz, t);
				VertexData I(1.0f - t, verts[i - 1], t, verts[i]);
				output.push_back(I);
				if (!v0In && v1In) {
					output.push_back(verts[i]);
				}
			}
		}
	}
	return output;
}

/**
 * @fn	std::vector<VertexData> VertexOps::clipPolygon(const std::vector<VertexData> &clipCoords)
 * @brief	Clip polygon against the normalized view volumn - 2x2x2 cube.
 * @param	clipCoords	The array of triangles.
 * @return	The array of triangles, after performing clipping.
 */

std::vector<VertexData> VertexOps::clipPolygon(const std::vector<VertexData> &clipCoords) {
	std::vector<VertexData> ndcCoords;

	if (clipCoords.size() > 2) {
		for (unsigned int i = 0; i < clipCoords.size() - 2; i += 3) {
			std::vector<VertexData> polygon;
			polygon.push_back(clipCoords[i]);
			polygon.push_back(clipCoords[i + 1]);
			polygon.push_back(clipCoords[i + 2]);

			for (IPlane plane : ndcPlanes) {
				polygon = clipAgainstPlane(polygon, plane);
			}
			if (polygon.size() > 3) {
				polygon = triangulate(polygon);
			}
			for (VertexData v : polygon) {
				ndcCoords.push_back(v);
			}
		}
	}
	return ndcCoords;
}

/**
 * @fn	std::vector<VertexData> VertexOps::clipLineSegments(const std::vector<VertexData> &clipCoords)
 * @brief	Clip line segments against normalized view volume.
 * @param	clipCoords	The vector of line segments that are to be clipped.
 * @return	A std::vector&lt;VertexData&gt;
 */

std::vector<VertexData> VertexOps::clipLineSegments(const std::vector<VertexData> &clipCoords) {
	std::vector<VertexData> ndcCoords;

	if (clipCoords.size() > 1) {
		for (unsigned int i = 0; i < clipCoords.size() - 1; i += 2) {
			VertexData v0 = clipCoords[i];
			VertexData v1 = clipCoords[i + 1];

			bool outsideViewVolume = false;

			for (const IPlane plane : ndcPlanes) {
				bool v0In = ndcPlanes[i].insidePlane(v0.position.xyz);
				bool v1In = ndcPlanes[i].insidePlane(v1.position.xyz);

				if (!v0In && !v1In) { // Line segment is entirely clipped
					outsideViewVolume = true;
					break; 
				} else if (v0In && !v1In) {
					float t;
					ndcPlanes[i].findIntersection(v0.position.xyz, v1.position.xyz, t);
					v1 = VertexData(1.0f-t, v0, t, v1);
				} else if (!v0In && v1In) {
					float t;
					ndcPlanes[i].findIntersection(v0.position.xyz, v1.position.xyz, t);
					v0 = VertexData(1.0f-t, v0, t, v1);
				} else {  // both inside
				}
			}
			if (!outsideViewVolume) {
				ndcCoords.push_back(v0);
				ndcCoords.push_back(v1);
			}
		}
	}
	return ndcCoords;
}

 /**
 * @fn	std::vector<VertexData> VertexOps::removeBackwardFacingTriangles(const std::vector<VertexData> &triangleVerts)
 * @brief	Removes the backward facing triangles
 * @param	triangleVerts	The vector of triangle vertices.
 * @return	Vector of triangle vertices, without those facing backward.
 */

std::vector<VertexData> VertexOps::removeBackwardFacingTriangles(const std::vector<VertexData> &triangleVerts) {
	std::vector<VertexData> fowardFacingTriangles;
	const glm::vec3 viewDirection(0.0f, 0.0f, -1.0f);

	for (int i = 0; i < (int)triangleVerts.size() - 2; i += 3) {
		glm::vec3 n = normalFrom3Points(triangleVerts[i].position.xyz, triangleVerts[i + 1].position.xyz, triangleVerts[i + 2].position.xyz);

		if (glm::dot(viewDirection, n) <= 0.0) {
			fowardFacingTriangles.push_back(triangleVerts[i]);
			fowardFacingTriangles.push_back(triangleVerts[i + 1]);
			fowardFacingTriangles.push_back(triangleVerts[i + 2]);
		}
	}
	return fowardFacingTriangles;
}

/**
 * @fn	std::vector<VertexData> VertexOps::transformVerticesToWorldCoordinates(const glm::mat4 &modelMatrix, const std::vector<VertexData> &vertices)
 * @brief	Apply modeling transformation to vector of vertices.
 * @param	modelMatrix	Modeling matrix.
 * @param	vertices   	The vector of vertices.
 * @return	The transformed vertices.
 */

std::vector<VertexData> VertexOps::transformVerticesToWorldCoordinates(const glm::mat4 &modelMatrix, const std::vector<VertexData> &vertices) {
	// Create 3 x 3 matrix for transforming normal vectors to world coordinates
	glm::mat3 TM3x3(modelMatrix);
	glm::mat3 modelingTransfomationForNormals = glm::transpose(glm::inverse(TM3x3));

	std::vector<VertexData> transformedVertices;
	for (unsigned int i=0; i<vertices.size(); i++) {
		const VertexData &v = vertices[i];
		glm::vec3 n = modelingTransfomationForNormals * v.normal;
		glm::vec4 worldPos = modelMatrix * v.position;
		VertexData vt(worldPos, n, v.material, worldPos.xyz);
		transformedVertices.push_back(vt);
	}
	return transformedVertices;
}

/**
 * @fn	void VertexOps::applyLighting(const std::vector<LightSourcePtr> &lights, std::vector<VertexData> &worldCoords)
 * @brief	Applies the lighting to all the vertices. Modifies the VertexData's material field.
 * @param 		  	lights	   	The vector of lights in the scene.
 * @param [in,out]	worldCoords	The vector of world coordinates.
 */

void VertexOps::applyLighting(const std::vector<LightSourcePtr> &lights, std::vector<VertexData> &worldCoords) {
	Frame eyeFrame = Frame::createOrthoNormalBasis(VertexOps::viewingTransformation);
	for (unsigned int i = 0; i < worldCoords.size(); i++) {
		VertexData &vert = worldCoords[i];
		float alpha = worldCoords[i].material.alpha;
		color totalLight = black;

		for (unsigned int j = 0; j < lights.size(); j++) {
			totalLight += lights[j]->illuminate(vert.worldPosition, vert.normal, vert.material,
												eyeFrame, false);
		}
		worldCoords[i].material.ambient = totalLight;
	}
}

/**
 * @fn	std::vector<VertexData> VertexOps::transformVertices(const glm::mat4 &TM, const std::vector<VertexData> & vertices)
 * @brief	Applies a transformation matrix to a vector of vertices. Does not change the worldPosition; copies it over.
 * @param	TM			The transformation matrix.
 * @param	vertices   	The vertices.
 * @return	The transformed vector of vertices.
 */

std::vector<VertexData> VertexOps::transformVertices(const glm::mat4 &TM, const std::vector<VertexData> & vertices) {
	std::vector<VertexData> transformedVertices;

	for (VertexData v : vertices) {
		VertexData vt(TM * v.position, v.normal, v.material);
		// Save the world position separately for use in per pixel lighting calculations
		vt.worldPosition = v.worldPosition;

		transformedVertices.push_back(vt);
	}
	return transformedVertices;
}

/**
 * @fn	void VertexOps::processTriangleVertices(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, const std::vector<VertexData> &objectCoords)
 * @brief	Transforms the triangle vertices through pipeline: object -> world -> eye -> clip/ndc -> window.
 * @param [in,out]	frameBuffer 	Buffer for frame data.
 * @param 		  	eyePos			The eye position.
 * @param 		  	lights			The lights.
 * @param 		  	objectCoords	The object coordinates.
 */

void VertexOps::processTriangleVertices(FrameBuffer &frameBuffer, const glm::vec3 &eyePos,
										const std::vector<LightSourcePtr> &lights,
										const std::vector<VertexData> &objectCoords) {
	std::vector<VertexData> worldCoords = transformVerticesToWorldCoordinates(modelingTransformation, objectCoords);
	std::vector<VertexData> eyeCoords = transformVertices(viewingTransformation, worldCoords);
	glm::mat4 VM = VertexOps::viewingTransformation;
	std::vector<VertexData> projCoords = transformVertices(projectionTransformation, eyeCoords);
	std::vector<VertexData> clipCoords;
	for (VertexData v : projCoords) {		// Perspective division
		if (v.position.w >= 0)
			v.position /= v.position.w;
		else {
			v.position.x /= -v.position.w;
			v.position.y /= -v.position.w;
			v.position.z = -std::abs(v.position.z);
			v.position.w = 1.0f;
		}
		clipCoords.push_back(v);
	}

	if (!renderBackFaces)	// backface culling?
		clipCoords = removeBackwardFacingTriangles(clipCoords);	

	std::vector<VertexData> ndcCoords = clipPolygon(clipCoords);
	std::vector<VertexData> windowCoords = transformVertices(viewportTransformation, ndcCoords);

	for (VertexData &vd : windowCoords) {
		vd.position.x = glm::clamp(vd.position.x, (float)viewport.lx, (float)viewport.rx);
		vd.position.y = glm::clamp(vd.position.y, (float)viewport.ly, (float)viewport.ry);
	}

	drawManyFilledTriangles(frameBuffer, eyePos, lights, windowCoords, viewingTransformation);
}

/**
 * @fn	void VertexOps::processTriangleVertices(FrameBuffer &frameBuffer, const std::vector<LightSourcePtr> &lights, const glm::mat4 &TM, const std::vector<VertexData> &objectCoords)
 * @brief	Process the triangle vertices
 * @param [in,out]	frameBuffer 	Framebuffer.
 * @param 		  	lights			The lights in the scene.
 * @param 		  	TM				The modeling transformation matrix.
 * @param 		  	objectCoords	The vector of triangles in object coordinates.
 */

void VertexOps::processTriangleVertices(FrameBuffer &frameBuffer,
										const std::vector<LightSourcePtr> &lights,
										const glm::mat4 &TM,
										const std::vector<VertexData> &objectCoords) {
	glm::vec3 eyePos = glm::inverse(VertexOps::viewingTransformation)[3].xyz;
	VertexOps::modelingTransformation = TM;
	processTriangleVertices(frameBuffer, eyePos, lights, objectCoords);
}

/**
 * @fn	void VertexOps::processLineSegments(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, const std::vector<VertexData> &objectCoords)
 * @brief	Process the line segments through the pipeline.
 * @param [in,out]	frameBuffer 	Frame buffer
 * @param 		  	eyePos			Eye position.
 * @param 		  	lights			The lights in the scene.
 * @param 		  	objectCoords	The vector of object coordinates.
 */

void VertexOps::processLineSegments(FrameBuffer &frameBuffer, const glm::vec3 &eyePos,
									const std::vector<LightSourcePtr> &lights,
									const std::vector<VertexData> &objectCoords) {
	std::vector<VertexData> worldCoords = transformVerticesToWorldCoordinates(modelingTransformation, objectCoords);

	std::vector<VertexData> eyeCoords = transformVertices(viewingTransformation, worldCoords);
	std::vector<VertexData> projCoords = transformVertices(projectionTransformation, eyeCoords);
	std::vector<VertexData> clipCoords;

	for (VertexData v : projCoords) {	// Perspective division
		if (v.position.w >= 0)
			v.position /= v.position.w;
		else {
			v.position /= -v.position.w;
			v.position.z = -std::abs(v.position.z);
		}
		clipCoords.push_back(v);
	}
	std::vector<VertexData> ndcCoords = clipLineSegments(clipCoords);
	std::vector<VertexData> windowCoords = transformVertices(viewportTransformation, ndcCoords);
	drawManyLines(frameBuffer, eyePos, lights, windowCoords, viewingTransformation);
}

/**
 * @fn	void VertexOps::render(FrameBuffer &frameBuffer, const std::vector<VertexData> verts, const std::vector<LightSourcePtr> &lights, const glm::mat4 &TM)
 * @brief	Renders this object
 * @param [in,out]	frameBuffer	Buffer for frame data.
 * @param 		  	verts	   	The vertices.
 * @param 		  	lights	   	The lights.
 * @param 		  	TM		   	The time.
 */

void VertexOps::render(FrameBuffer &frameBuffer, const std::vector<VertexData> verts,
							const std::vector<LightSourcePtr> &lights,
							const glm::mat4 &TM) {
	glm::vec3 eyePos = glm::inverse(VertexOps::viewingTransformation)[3].xyz;
	VertexOps::modelingTransformation = TM;
	VertexOps::processTriangleVertices(frameBuffer, eyePos, lights, verts);
}

/**
 * @fn	void VertexOps::setViewport(float left, float right, float bottom, float top)
 * @brief	Sets a viewport to a particular setting.
 * @param	left  	
 * @param	right 	
 * @param	bottom	
 * @param	top   
 */

void VertexOps::setViewport(int left, int right, int bottom, int top) {
	setViewport(BoundingBoxi(left, right, bottom, top));
}

/**
 * @fn	void VertexOps::setViewport(const BoundingBox &vp)
 * @brief	Sets a viewport to a particlar setting
 * @param	vp	The new viewport settings
 */

void VertexOps::setViewport(const BoundingBoxi &vp) {
	viewport = vp;
	setViewportTransformation();
}

/**
 * @fn	void VertexOps::setViewportTransformation()
 * @brief	Sets viewport transformation based on the current viewport settings.
 */

void VertexOps::setViewportTransformation() {
	VertexOps::viewportTransformation = T((float)VertexOps::viewport.lx,
											(float)VertexOps::viewport.ly, 0.0f) *
		S((float)VertexOps::viewport.width() / VertexOps::ndc.width(),
			(float)VertexOps::viewport.height() / VertexOps::ndc.height(), 1.0f) *
		T(-VertexOps::ndc.lx, -VertexOps::ndc.ly, 0.0f);
}