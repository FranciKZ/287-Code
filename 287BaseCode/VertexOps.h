#pragma once

#include "Defs.h"
#include "FrameBuffer.h"
#include "Light.h"
#include "VertexData.h"
#include "IScene.h"
#include "Rasterization.h"

/**
 * @class	VertexOps
 * @brief	Class to encapsulate the methods related to vertex processing.
 */

class VertexOps {
public:
	static bool renderBackFaces;				//!< Typically false for closed body objects (e.g., sphere).
	static glm::mat4 modelingTransformation;	//!< Used to orient/scale/position objects. Changed often.
	static glm::mat4 viewingTransformation;		//!< Orient/position camera.
	static glm::mat4 projectionTransformation;	//!< Define projection. Typically set just once.
	static glm::mat4 viewportTransformation;	//!< Controls where NDCs map onto window.

	static const BoundingBox3D ndc;				//!< normalized device coordinate; the limits

	static std::vector<IPlane> ndcPlanes;		//!< the 6 planes of the 2x2x2 cube.

	static void processTriangleVertices(FrameBuffer &frameBuffer, const glm::vec3 &eyePos,
										const std::vector<LightSourcePtr> &lights,
										const std::vector<VertexData> &objectCoords);
	static void processTriangleVertices(FrameBuffer &frameBuffer,
										const std::vector<LightSourcePtr> &lights,
										const glm::mat4 &TM,
										const std::vector<VertexData> &objectCoords);
	static void processLineSegments(FrameBuffer &frameBuffer, const glm::vec3 &eyePos,
									const std::vector<LightSourcePtr> &lights,
									const std::vector<VertexData> &objectCoords);
	static void VertexOps::render(FrameBuffer &frameBuffer, const std::vector<VertexData> verts,
								const std::vector<LightSourcePtr> &lights,
								const glm::mat4 &TM);
	static void setViewport(int left, int right, int bottom, int top);
	static void setViewport(const BoundingBoxi &vp);
protected:
	static BoundingBoxi viewport;			//!< the currently active viewport
	static void setViewportTransformation();
	static std::vector<VertexData> clipAgainstPlane(std::vector<VertexData> &verts, const IPlane &plane);
	static std::vector<VertexData> clipPolygon(const std::vector<VertexData> &clipCoords);
	static std::vector<VertexData> clipLineSegments(const std::vector<VertexData> &clipCoords);
	static std::vector<VertexData> removeBackwardFacingTriangles(const std::vector<VertexData> &triangleVerts);
	static std::vector<VertexData> transformVerticesToWorldCoordinates(const glm::mat4 &modelMatrix, const std::vector<VertexData> &vertices);
	static void applyLighting(const std::vector<LightSourcePtr> &lights, std::vector<VertexData> &worldCoords);
	static std::vector<VertexData> transformVertices(const glm::mat4 &TM, const std::vector<VertexData> &vertices);
};
