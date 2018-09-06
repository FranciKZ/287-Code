#pragma once

#include "Defs.h"
#include "FragmentOps.h"
#include "VertexData.h"

void drawAxisOnWindow(FrameBuffer &frameBuffer);
void drawWirePolygon(FrameBuffer &frameBuffer, const std::vector<glm::vec3> &pts, const color &rgb);
void drawLine(FrameBuffer &frameBuffer, int x1, int y1, int x2, int y2, const color &C);
void drawLine(FrameBuffer &frameBuffer, const glm::vec2 &pt1, const glm::vec2 &pt2, const color &C);
void drawLine(FrameBuffer &frameBuffer, const glm::vec3 &eyePos,
				std::vector<LightSourcePtr> &lights, 
				const VertexData &v0, const VertexData &v1,
				const glm::mat4 &viewingMatrix);
void drawManyLines(FrameBuffer &frameBuffer, const glm::vec3 &eyePos,
					const std::vector<LightSourcePtr> &lights, const std::vector<VertexData> &vertices,
					const glm::mat4 &viewingMatrix);
void drawWireFrameTriangle(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, const std::vector<LightSourcePtr> &lights, const VertexData &v0, const VertexData &v1, const VertexData &v2,
							const glm::mat4 &viewingMatrix);
void drawFilledTriangle(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, std::vector<LightSourcePtr> &lights, const VertexData &v0, const VertexData &v1, const VertexData &v2,
						const glm::mat4 &viewingMatrix);
void drawManyWireFrameTriangles(FrameBuffer &frameBuffer, const glm::vec3 &eyePos, 
								const std::vector<LightSourcePtr> &lights, const std::vector<VertexData> &vertices,
								const glm::mat4 &viewingMatrix);
void drawManyFilledTriangles(FrameBuffer &frameBuffer, const glm::vec3 &eyePos,
							const std::vector<LightSourcePtr> &lights, const std::vector<VertexData> &vertices,
								const glm::mat4 &viewingMatrix);
void drawArc(FrameBuffer &fb, const glm::vec2 &center, float R,
	float startRads, float lengthInRads, const color &rgb);
