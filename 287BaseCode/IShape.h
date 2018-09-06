#pragma once
#include <vector>
#include "HitRecord.h"

struct IShape;
typedef IShape *IShapePtr;
struct VisibleIShape;
typedef VisibleIShape *VisibleIShapePtr;

/**
 * @struct	Ray
 * @brief	Represents a ray.
 */

struct Ray {
	glm::vec3 origin;		//!< starting point for this ray
	glm::vec3 direction;	//!< direction for this ray, given it's origin
	Ray(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection) :
		origin(rayOrigin), direction(glm::normalize(rayDirection)) {
	}
	glm::vec3 getPoint(float t) const {
		return origin + t * direction;
	}
};

/**
 * @struct	IShape
 * @brief	Base class for all implicit shapes.
 */

struct IShape {
	IShape();
	virtual void findClosestIntersection(const Ray &ray, HitRecord &hit) const = 0;
	virtual void getTexCoords(const glm::vec3 &pt, float &u, float &v) const;
	static glm::vec3 movePointOffSurface(const glm::vec3 &pt, const glm::vec3 &n);
};

/**
 * @struct	VisibleIShape
 * @brief	A visible implicit shape.
 */

struct VisibleIShape {
	Material material;	//!< Material for this shape.
	IShapePtr shape;	//!< Pointer to underlying implicit shape.
	Image *texture;		//!< Texture associated with this shape, if any.
	float lu;			//!< left u value
	float ru;			//!< right u value
	float lv;			//!< left v value
	float rv;			//!< right v value
	VisibleIShape(IShapePtr shapePtr, const Material &mat);
	void findClosestIntersection(const Ray &ray, HitRecord &hit) const;
	void setTexture(Image *tex, float leftU, float rightU, float bottomV, float topV);
	void setTexture(Image *tex);
	static HitRecord findIntersection(const Ray &ray, const std::vector<VisibleIShapePtr> &surfaces);
};

/**
 * @struct	IPlane
 * @brief	An implicit representation of a plane.
 */

struct IPlane : public IShape {
	glm::vec3 a;	//!< point on the plane
	glm::vec3 n;	//!< plane's normal vector
	IPlane(const glm::vec3 &point, const glm::vec3 &normal);
	IPlane(const std::vector<glm::vec3> &vertices);
	IPlane(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3);
	virtual void findClosestIntersection(const Ray &ray, HitRecord &hit) const;
	bool insidePlane(const glm::vec3 &point) const;
	void findIntersection(const glm::vec3 &p1, const glm::vec3 &p2, float &t) const;
};

/**
 * @struct	IDisk
 * @brief	Implicit representation of a disk (i.e., 2D circle) with a particular
 * 			center and normal vector.
 */

struct IDisk : public IShape {
	IDisk(const glm::vec3 &position, const glm::vec3 &n, float rad);
	virtual void findClosestIntersection(const Ray &ray, HitRecord &hit) const;
	glm::vec3 center;	//!< center point of disk
	glm::vec3 n;		//!< normal vector of disk
	float radius;
};

/**
 * @struct	IRect
 * @brief	Implicit representation of a rectangle, with a particular center
 * 			and normal vector.
 */

struct IRect : public IShape {
	IRect(const glm::vec3 &position, const glm::vec3 &normal, float W, float H);
	virtual void findClosestIntersection(const Ray &ray, HitRecord &hit) const;
	float width;		//!< width of rectangle
	float height;		//!< height of rectangle
	glm::vec3 center;	//!< center point of rectangle
protected:
	float W2;			//!< width/2
	float H2;			//!< height/2
	glm::vec3 n;		//!< normal vector of rectangle
	IPlane plane;		//!< infinite plane that subsumes the given finite plane
};

/**
 * @struct	IBox
 * @brief	Implicit representation of 3D box.
 */

struct IBox : public IShape {
	IBox(const glm::vec3 &center, const glm::vec3 &size);
	IBox(const glm::vec3 &center, float size);
	virtual void findClosestIntersection(const Ray &ray, HitRecord &hit) const;
protected:
	std::vector<IRect> rects;	//!< 6 rectangles corresponding to sides of box.
};

/**
 * @struct	QuadricParameters
 * @brief	Represents the 9 parameters that describe a quadric.
 */

struct QuadricParameters {
	float A, B, C, D, E, F, G, H, I, J;
	QuadricParameters();
	QuadricParameters(const std::vector<float> &items);
	QuadricParameters(float a, float b, float c, float d, float e, float f,
		float g, float h, float i, float j);
	static QuadricParameters cylinderXQParams(float R);
	static QuadricParameters cylinderYQParams(float R);
	static QuadricParameters cylinderZQParams(float R);
	static QuadricParameters sphereQParams(float R);
	static QuadricParameters ellipsoidQParams(const glm::vec3 &sz);
};

/**
 * @struct	IConvexPolygon
 * @brief	Implicit representation for a convex polygon.
 */

struct IConvexPolygon : public IPlane {
	std::vector<glm::vec3> v;
	glm::vec3 n;
	IConvexPolygon(const std::vector<glm::vec3> &vertices);
	virtual void findClosestIntersection(const Ray &ray, HitRecord &hit) const;
	bool isInside(const glm::vec3 &point) const;
};

/**
 * @struct	ITriangle
 * @brief	Implicit representation of triangle.
 */

struct ITriangle : public IShape {
	glm::vec3 a;	//!< first vertex.
	glm::vec3 b;	//!< second vertex.
	glm::vec3 c;	//!< third vertex.
	IPlane plane;	//!< the plane this triangle lies on.
	ITriangle(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C);
	virtual void findClosestIntersection(const Ray &ray, HitRecord &hit) const;
	bool inside(const glm::vec3 &pt) const;
};

/**
 * @struct	IQuadricSurface
 * @brief	Implicit representation of quadric surface. These shapes can be
 * 			described by the general quadric surface equation
 */

struct IQuadricSurface : public IShape {
	glm::vec3 center;	//!< center of quadric
	IQuadricSurface(const QuadricParameters &params,
					const glm::vec3 &position);
	IQuadricSurface(const std::vector<float> &params,
					const glm::vec3 & position);
	IQuadricSurface(const glm::vec3 & position = glm::vec3(0, 0, 0));
	virtual void findClosestIntersection(const Ray &ray, HitRecord &hit) const;
	int findIntersections(const Ray &ray, HitRecord hits[2]) const;
	glm::vec3 normal(const glm::vec3 &pt) const;
	virtual void computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const;
protected:
	QuadricParameters qParams;		//!< The parameters that make up the quadric
	float twoA;						//!< 2*A
	float twoB;						//!< 2*B
	float twoC;						//!< 2*C
};

/**
 * @struct	ISphere
 * @brief	Implicit representation of sphere.
 */

struct ISphere : IQuadricSurface {
	ISphere(const glm::vec3 &position, float radius);
	virtual void getTexCoords(const glm::vec3 &pt, float &u, float &v) const;
	virtual void computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const;
};

/**
 * @struct	ICylinder
 * @brief	Base class for implicit representation of a cylinder.
 */

struct ICylinder : public IQuadricSurface {
	float radius, length;
	ICylinder(const glm::vec3 &position, float R, float len, const QuadricParameters &qParams);
	virtual void findClosestIntersection(const Ray &ray, HitRecord &hit) const = 0;
	virtual void computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const;
};

/**
 * @struct	ICylinderY
 * @brief	Implicit representation of open cylinder oriented along y-axis coordinate.
 */

struct ICylinderY : public ICylinder {
	ICylinderY(const glm::vec3 &position, float R, float len);
	virtual void findClosestIntersection(const Ray &ray, HitRecord &hit) const;
	void getTexCoords(const glm::vec3 &pt, float &u, float &v) const;
};

/**
 * @struct	IEllipsoid
 * @brief	Implicit representation of an ellipsoid.
 */

struct IEllipsoid : public IQuadricSurface {
	IEllipsoid(const glm::vec3 &position, const glm::vec3 &sz);
	virtual void computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const;
};