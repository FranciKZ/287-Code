#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "Defs.h"
#include "ColorAndMaterials.h"

extern bool DEBUG_PIXEL;
extern int xDebug, yDebug;
void mouseUtility(int, int, int, int);

// Simple streaming for vectors and matrices.
std::ostream &operator << (std::ostream &os, const glm::vec2 &v);
std::ostream &operator << (std::ostream &os, const glm::vec3 &v);
std::ostream &operator << (std::ostream &os, const glm::vec4 &v);
std::ostream &operator << (std::ostream &os, const glm::mat2 &v);
std::ostream &operator << (std::ostream &os, const glm::mat3 &v);
std::ostream &operator << (std::ostream &os, const glm::mat4 &v);
std::ostream &operator << (std::ostream &os, const glm::mat4x2 &v);

template <class T>
std::ostream &operator << (std::ostream &os, const std::vector<T> &V) {
	os << "[" << std::endl;
	for (unsigned int i = 0; i < V.size(); i++) {
		os << '\t' << V[i] << std::endl;
	}
	os << "]" << std::endl;
	return os;
}

void swap(float &a, float &b);
bool approximatelyEqual(float a, float b);
bool approximatelyZero(float a);
float normalizeDegrees(float degrees);
float normalizeRadians(float rads);
float rad2deg(float rads);
float deg2rad(float degs);
float min(float a, float b, float c);
float max(float a, float b, float c);
void pointOnUnitCircle(float angleRads, float &x, float &y);

glm::vec2 pointOnCircle(const glm::vec2 &center, float R, float angleRads);
float directionInDegrees(float x1, float y1, float x2, float y2);
float directionInDegrees(const glm::vec2 &targetPt);
float directionInDegrees(const glm::vec2 &referencePt, const glm::vec2 &targetPt);

glm::vec2 doubleIt(const glm::vec2 &V);
glm::vec3 myNormalize(const glm::vec3 &V);
bool isOrthogonal(const glm::vec3 &a, const glm::vec3 &b);
float cosBetween(const glm::vec2 &v1, const glm::vec2 &v2);
float cosBetween(const glm::vec3 &v1, const glm::vec3 &v2);
float radsBetween(const glm::vec2 &v1, const glm::vec2 &v2);
float radsBetween(const glm::vec3 &v1, const glm::vec3 &v2);
glm::vec3 project(const glm::vec3 &from, const glm::vec3 &onto);
glm::vec2 rotate90CCW(const glm::vec2 &pt);

float map(float x, float xLow, float xHigh, float yLow, float yHigh);
void map(float x, float xLow, float xHigh, float yLow, float yHigh, float &y);

std::vector<float> quadratic(float A, float B, float C);
int quadratic(float A, float B, float C, float roots[2]);

float areaOfParallelogram(const glm::vec3 &v1, const glm::vec3 &v2);
float areaOfTriangle(const glm::vec3 &pt1, const glm::vec3 &pt2, const glm::vec3 &pt3);


bool isOrthoNormalBasis(const glm::vec3 &u, const glm::vec3 &v, const glm::vec3 &w);
bool isRightHandedOrthoNormalBasis(const glm::vec3 &u, const glm::vec3 &v, const glm::vec3 &w);
glm::vec3 pointingVector(const glm::vec3 &pt1, const glm::vec3 &pt2);
glm::vec3 normalFrom3Points(const glm::vec3 &pt1, const glm::vec3 &pt2, const glm::vec3 &pt3);
glm::vec3 normalFrom3Points(const std::vector<glm::vec3> pts);
glm::vec3 getRow(const glm::mat3 &mat, int row);
glm::vec3 getCol(const glm::mat3 &mat, int col);
bool isInvertible(const glm::mat2 &mat);
bool isInvertible(const glm::mat3 &mat);
glm::vec3 solveLinearSystem(const glm::mat3 &M, const glm::vec3 &y);
glm::mat3 addMatrices(const std::vector<glm::mat3> &M);
glm::mat3 multiplyMatrices(const std::vector<glm::mat3> &M);
glm::vec3 multiplyMatrixAndVertex(const glm::mat3 &M, const glm::vec3 &x);
glm::vec3 multiplyMatricesAndVertex(const std::vector<glm::mat3> &M, const glm::vec3 &x);
std::vector<glm::vec3> multiplyMatrixAndVertices(const glm::mat3 &M, const std::vector<glm::vec3> &verts);
std::vector<glm::vec3> multiplyMatricesAndVertices(const std::vector<glm::mat3> &M, const std::vector<glm::vec3> &verts);
glm::mat2 myinverse(const glm::mat2 &M);

void computeXYZFromAzimuthAndElevation(float R,
	float az, float el,
	float &x, float &y, float &z);
void computeAzimuthAndElevationFromXYZ(float x, float y, float z,
	float &R, float &az, float &el);
void computeAzimuthAndElevationFromXYZ(const glm::vec3 &pt,
	float &R, float &az, float &el);

std::string extractBaseFilename(const std::string &str);

// 2D versions
glm::mat3 T(float dx, float dy);
glm::mat3 S(float sx, float sy);
glm::mat3 R(float rads);
glm::mat3 vertShear(float a);
glm::mat3 horzShear(float a);

// 3D versions
glm::mat4 T(float dx, float dy, float dz);
glm::mat4 S(float sx, float sy, float sz);
glm::mat4 S(float scale);
glm::mat4 Rx(float rads);
glm::mat4 Ry(float rads);
glm::mat4 Rz(float rads);

bool inRangeInclusive(float val, float lo, float hi);
bool inRangeExclusive(float val, float lo, float hi);
bool inRectangle(float x, float y, float left, float bottom, float right, float top);
bool inRectangle(const glm::vec2 &pt, const glm::vec2 &lowerLeft, const glm::vec2 &upperRight);
bool approximateVectorEquality(const glm::vec3 &v1, const glm::vec3 &v2);

/**
 * @fn	template <class T> std::vector<std::vector<T>> allocate2D(int rows, int cols)
 * @brief	Allocates a vector of vectors.
 * @tparam	T	Generic type parameter.
 * @param	rows	The number rows.
 * @param	cols	The number cols.
 * @return	The vector of vectors, having the proper dimensions.
 */

template <class T>
std::vector<std::vector<T>> allocate2D(int rows, int cols) {
	std::vector<std::vector<T>> result(rows);
	for (int r = 0; r < rows; r++) {
		result[r] = std::vector<T>(cols);
	}
	return result;
}

/**
 * @fn	template <class T> T weightedAverage(float W1, const T &item1, float W2, const T &item2)
 * @brief	Computes the weighted average of two items.
 * @tparam	T	Generic type parameter.
 * @param	W1   	The first weight.
 * @param	item1	The first item.
 * @param	W2   	The second weight.
 * @param	item2	The second item.
 * @return	The weighted average.
 */

template <class T>
T weightedAverage(float W1, const T &item1, float W2, const T &item2) {
	return item1 * W1 + item2 * W2;
}

/**
 * @fn	template <class T> T average(const T &a, const T &b)
 * @brief	Computes the average of two items.
 * @tparam	T	Generic type parameter.
 * @param	a	First item.
 * @param	b	Second item.
 * @return	The average value.
 */

template <class T>
T average(const T &a, const T &b) {
	return (a + b) / 2.0f;
}

/**
 * @fn	template <class T> T average(const T &a, const T &b , const T &c, const T &d)
 * @brief	Computes the average of four items.
 * @tparam	T	Generic type parameter.
 * @param	a	First item.
 * @param	b	Second item.
 * @param	c	Third item.
 * @param	d	Fourth item.
 * @return	The average value.
 */

template <class T>
T average(const T &a, const T &b , const T &c, const T &d) {
	return (a + b + c + d) / 4.0f;
}

/**
 * @fn	template <class T1, class T2> void addAll(std::vector<T1> &vec, const std::vector<T2> &newItems)
 * @brief	Adds all the elements in newItems to end of the vector vec.
 * @tparam	T	Generic type parameter.
 * @param [in,out]	vec			The destination vector.
 * @param 		  	newItems	The new items to be added.
 */

template <class T>
void addAll(std::vector<T> &vec, const std::vector<T> &newItems) {
	for (std::vector<T>::const_iterator i = newItems.begin(); i != newItems.end(); i++) {
		vec.push_back(*i);
	}
}
