#include <vector>
#include "IShape.h"

/**
 * @fn	IShape::IShape()
 * @brief	Constructs a default IShape, centered at the origin.
 */

IShape::IShape() {
}

/**
 * @fn	void IShape::getTexCoords(const glm::vec3 &pt, float &u, float &v) const
 * @brief	Computes the tex coordinate of a point on the surface. The default
 * 			return value is (0, 0)
 * @param 		  	pt	The coordinate to process
 * @param [in,out]	u 	The u, in (u, v).
 * @param [in,out]	v 	The v, in (u, v).
 */

void IShape::getTexCoords(const glm::vec3 &pt, float &u, float &v) const {
	u = v = 0;
}

/**
 * @fn	glm::vec3 IShape::movePointOffSurface(const glm::vec3 &pt, const glm::vec3 &n)
 * @brief	Compute point that is slightly off surface.
 * @param	pt	Intersection point.
 * @param	n 	Normal vector at pt.
 * @return	The point that is approximately EPSILON off the surface.
 */

glm::vec3 IShape::movePointOffSurface(const glm::vec3 &pt, const glm::vec3 &n) {
	return pt;
}

/**
 * @fn	VisibleIShape::VisibleIShape(IShapePtr shapePtr, const Material &mat)
 * @brief	Represents an visible, implicit shape.
 * @param	shapePtr	Pointer to the implicit shape.
 * @param	mat			Material
 */

VisibleIShape::VisibleIShape(IShapePtr shapePtr, const Material &mat)
	: material(mat), shape(shapePtr) {
	texture = nullptr;
	lu = lv = 0.0f;
	ru = rv = 1.0f;
}

/**
 * @fn	void VisibleIShape::findClosestIntersection(const Ray &ray, HitRecord &hit) const
 * @brief	Identifies the closest intersection
 * @param 		  	ray	The ray.
 * @param [in,out]	hit	The hit that repesents the closest "hit".
 */

void VisibleIShape::findClosestIntersection(const Ray &ray, HitRecord &hit) const {
	shape->findClosestIntersection(ray, hit);
	if (hit.t < FLT_MAX) {
		hit.material = material;
	}
}

/**
 * @fn	void VisibleIShape::setTexture(Image *tex, float leftU, float rightU, float bottomV, float topV)
 * @brief	Sets the texture for this implicit shape.
 * @param [in,out]	tex	   	The image to use as the texture.
 * @param 		  	leftU  	The left u.
 * @param 		  	rightU 	The right u.
 * @param 		  	bottomV	The bottom v.
 * @param 		  	topV   	The top v.
 */

void VisibleIShape::setTexture(Image *tex, float leftU, float rightU, float bottomV, float topV) {
	texture = tex;
	lu = leftU;
	ru = rightU;
	lv = bottomV;
	rv = topV;
}

/**
 * @fn	void VisibleIShape::setTexture(Image *tex)
 * @brief	Sets a texture for this implicit shape.
 * @param [in,out]	tex	The image to use as the texture, if not null.
 */

void VisibleIShape::setTexture(Image *tex) {
	setTexture(tex, 0.0f, 0.0f, 1.0f, 1.0f);
}

/**
 * @fn	HitRecord VisibleIShape::findIntersection(const Ray &ray, const std::vector<VisibleIShapePtr> &surfaces)
 * @brief	Searches for the first intersection
 * @param	ray			The ray.
 * @param	surfaces	The surfaces in the scene.
 * @return	The closest intersection that is in front of the camera.
 */

HitRecord VisibleIShape::findIntersection(const Ray &ray, const std::vector<VisibleIShapePtr> &surfaces) {
	HitRecord theHit;
	theHit.t = FLT_MAX;

	for (int i = 0; i < surfaces.size(); i++) {
		HitRecord thisHit;
		surfaces[i]->findClosestIntersection(ray, thisHit);
		if (thisHit.t < theHit.t && thisHit.t > 0) {
			theHit = thisHit;
			theHit.material = surfaces[i]->material;
			theHit.texture = surfaces[i]->texture;
			if (theHit.texture != nullptr) {
				surfaces[i]->shape->getTexCoords(theHit.interceptPoint, theHit.u, theHit.v);
			}
		}
	}
	return theHit;

}

/**
 * @fn	IDisk::IDisk(const glm::vec3 &pos, const glm::vec3 &normal, float rad)
 * @brief	Implicit representation of an implicit disk.
 * @param	pos   	Center of disk.
 * @param	normal	Normal vector of disk.
 * @param	rad   	Radius of disk.
 */

IDisk::IDisk(const glm::vec3 &pos, const glm::vec3 &normal, float rad)
	: IShape(), center(pos), n(normal), radius(rad) {
}

/**
 * @fn	void IDisk::findClosestIntersection(const Ray &ray, HitRecord &hit) const
 * @brief	Identifies the nearest intersection
 * @param 		  	ray	The ray.
 * @param [in,out]	hit	The hit.
 */

void IDisk::findClosestIntersection(const Ray &ray, HitRecord &hit) const {
	IPlane p(center, n);
	p.findClosestIntersection(ray, hit);
	if (glm::distance(hit.interceptPoint, center) > radius) {
		hit.t = FLT_MAX;
	}
}

/**
 * @fn	ISphere::ISphere(const glm::vec3 & position, float radius)
 * @brief	Implicit representation of a 3D sphere.
 * @param	position	The center of the sphere.
 * @param	radius  	The radius of the sphere.
 */

ISphere::ISphere(const glm::vec3 &position, float radius)
	: IQuadricSurface(QuadricParameters::sphereQParams(radius), position) {
}

/**
 * @fn	void ISphere::getTexCoords(const glm::vec3 &pt, float &u, float &v) const
 * @brief	Gets texture coordinates for a point on the surface.
 * @param 		  	pt	The point on the surface.
 * @param [in,out]	u 	The u in the (u, v) texture coordinates.
 * @param [in,out]	v 	The v in the (u, v) texture coordinates.
 */

void ISphere::getTexCoords(const glm::vec3 &pt, float &u, float &v) const {
	u = v = 0.0f;
}

/**
 * @fn	void ISphere::computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const
 * @see textbook.
 * @brief	Calculates aq, bq, and cq that are used in the quadric equations. The 
 * 			values that are know to be zero are eliminated from the code.
 * @param 		  	ray	The ray.
 * @param [in,out]	Aq 	aq.
 * @param [in,out]	Bq 	bq.
 * @param [in,out]	Cq 	cq.
 */

void ISphere::computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const {
	const float &A = qParams.A;
	const float &B = qParams.B;
	const float &C = qParams.C;
	const float &D = qParams.D;
	const float &E = qParams.E;
	const float &F = qParams.F;
	const float &G = qParams.G;
	const float &H = qParams.H;
	const float &I = qParams.I;
	const float &J = qParams.J;

	glm::vec3 Ro = ray.origin - center;
	const glm::vec3 &Rd = ray.direction;

	Aq = /*A * */ (Rd.x*Rd.x) +
		/*B * */ (Rd.y*Rd.y) +
		/*C * */ (Rd.z*Rd.z);
		//D * (Rd.x * Rd.y) +
		//E * (Rd.x * Rd.z) +
		//F * (Rd.y * Rd.z);

	Bq = 2 * /* A * */ Ro.x*Rd.x +
		2 * /* B * */ Ro.y*Rd.y +
		2 * /* C * */ Ro.z*Rd.z;
		//D * (Ro.x * Rd.y + Ro.y * Rd.x) +
		//E * (Ro.x * Rd.z + Ro.z * Rd.x) +
		//F * (Ro.y * Rd.z + Ro.z * Rd.y) +
		//G * Rd.x + H * Rd.y + I * Rd.z;

	Cq = /* A * */ (Ro.x * Ro.x) +
		/* B * */ (Ro.y * Ro.y) +
		/* C * */ (Ro.z * Ro.z) +
		//D * (Ro.x * Ro.y) +
		//E * (Ro.x * Ro.z) +
		//F * (Ro.y * Ro.z) +
		//G * Ro.x +
		//H * Ro.y +
		I * Ro.z + J;
}

/**
 * @fn	IBox::IBox(const glm::vec3 &center, const glm::vec3 &size)
 * @brief	Implicit representation of a 3D box.
 * @param	center	The center of the box.
 * @param	size  	The size of the box.
 */

IBox::IBox(const glm::vec3 &center, const glm::vec3 &size) 
		: IShape() {
	glm::vec3 sizeDiv2 = 0.5f * size;
	rects.push_back(IRect(center + glm::vec3(sizeDiv2.x, 0, 0), glm::vec3(1, 0, 0), size.y, size.z));
	rects.push_back(IRect(center - glm::vec3(sizeDiv2.x, 0, 0), glm::vec3(-1, 0, 0), size.y, size.z));
	rects.push_back(IRect(center + glm::vec3(0, sizeDiv2.y, 0), glm::vec3(0, 1, 0), size.x, size.z));
	rects.push_back(IRect(center - glm::vec3(0, sizeDiv2.y, 0), glm::vec3(0, -1, 0), size.x, size.z));
	rects.push_back(IRect(center + glm::vec3(0, 0, sizeDiv2.z), glm::vec3(0, 0, 1), size.x, size.y));
	rects.push_back(IRect(center - glm::vec3(0, 0, sizeDiv2.z), glm::vec3(0, 0, -1), size.x, size.y));
}

/**
 * @fn	IBox::IBox(const glm::vec3 &center, float size) : IBox(center, glm::vec3(size, size, size))
 * @brief	Constructs a 3D box.
 * @param	center	The center of the box.
 * @param	size  	The size of the box.
 */

IBox::IBox(const glm::vec3 &center, float size)
	: IBox(center, glm::vec3(size, size, size)) {
}

/**
 * @fn	void IBox::findClosestIntersection(const Ray &ray, HitRecord &theHit) const
 * @brief	Identifies the nearest intersection
 * @param 		  	ray	The ray.
 * @param [in,out]	theHit	The hit.
 */

void IBox::findClosestIntersection(const Ray &ray, HitRecord &theHit) const {
	for (unsigned int i = 0; i < rects.size(); i++) {
		HitRecord thisHit;
		rects[i].findClosestIntersection(ray, thisHit);
		if (thisHit.t < theHit.t) {
			theHit = thisHit;
		}
	}
}

/**
 * @fn	QuadricParameters::QuadricParameters() : QuadricParameters(std::vector<float> {1, 1, 1, 0, 0, 0, 0, 0, 0, -1})
 * @brief	Default constructor
 */

QuadricParameters::QuadricParameters()
	: QuadricParameters(std::vector<float> {1, 1, 1, 0, 0, 0, 0, 0, 0, -1}) {
}

/**
 * @fn	QuadricParameters::QuadricParameters(const std::vector<float> &items)
 * @brief	Constructor using 10 values.
 * @param	items	The items.
 */

QuadricParameters::QuadricParameters(const std::vector<float> &items)
	: A(items[0]), B(items[1]), C(items[2]), D(items[3]),
	E(items[4]), F(items[5]), G(items[6]), H(items[7]),
	I(items[8]), J(items[9]) {
}

/**
 * @fn	QuadricParameters::QuadricParameters(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j)
 * @brief	Constructor
 * @param	a	Quadric parameter A.
 * @param	b	Quadric parameter B.
 * @param	c	Quadric parameter C.
 * @param	d	Quadric parameter D.
 * @param	e	Quadric parameter E.
 * @param	f	Quadric parameter F.
 * @param	g	Quadric parameter G.
 * @param	h	Quadric parameter H.
 * @param	i	Quadric parameter I.
 * @param	j	Quadric parameter J.
 */

QuadricParameters::QuadricParameters(float a, float b, float c, float d, float e, float f,
	float g, float h, float i, float j)
	: QuadricParameters(std::vector<float> {a, b, c, d, e, f, g, h, i, j}) {
}

/**
 * @fn	QuadricParameters QuadricParameters::cylinderXQParams(float R)
 * @brief	Constructs the parameters for a cylinder oriented along the x axis.
 * @param	R	Radius of cylinder.
 * @return	The QuadricParameters.
 */

QuadricParameters QuadricParameters::cylinderXQParams(float R) {
	float R2 = R * R;
	return QuadricParameters(0.0f, 1.0f / R2, 1.0f / R2, 0, 0, 0, 0, 0, 0, -1);
}

/**
 * @fn	QuadricParameters QuadricParameters::cylinderYQParams(float R)
 * @brief	Constructs the parameters for a cylinder oriented along the y axis.
 * @param	R	Radius of cylinder.
 * @return	The QuadricParameters.
 */

QuadricParameters QuadricParameters::cylinderYQParams(float R) {
	float R2 = R * R;
	return QuadricParameters(1.0f / R2, 0, 1.0f / R2, 0, 0, 0, 0, 0, 0, -1);
}

/**
 * @fn	QuadricParameters QuadricParameters::cylinderZQParams(float R)
 * @brief	Constructs the parameters for a cylinder oriented along the z axis.
 * @param	R	Radius of cylinder.
 * @return	The QuadricParameters.
 */

QuadricParameters QuadricParameters::cylinderZQParams(float R) {
	float R2 = R * R;
	return QuadricParameters(1.0f / R2, 1.0f / R2, 0, 0, 0, 0, 0, 0, 0, -1);
}

/**
 * @fn	QuadricParameters QuadricParameters::sphereQParams(float R)
 * @brief	Constructs the parameters for a sphere centered on the origin.
 * @param	R	Radius of cylinder.
 * @return	The QuadricParameters.
 */

QuadricParameters QuadricParameters::sphereQParams(float R) {
	float R2 = R * R;
	return QuadricParameters(1, 1, 1, 0, 0, 0, 0, 0, 0, -R2);
}

/**
 * @fn	QuadricParameters QuadricParameters::ellipsoidQParams(glm::vec3 sz)
 * @brief	Ellipoid parameters
 * @param	sz	Size of ellipsoid.
 * @return	The QuadricParameters.
 */

QuadricParameters QuadricParameters::ellipsoidQParams(const glm::vec3 &sz) {
	glm::vec3 size = 1.0f / (sz * sz);
	return QuadricParameters(size.x, size.y, size.z, 0, 0, 0, 0, 0, 0, -1);
}

/**
 * @fn	void IPlane::findClosestIntersection(const Ray &ray, HitRecord &hit) const
 * @brief	Implicit representation of a plane
 * @param 		  	ray	The ray.
 * @param [in,out]	hit	The hit.
 */

void IPlane::findClosestIntersection(const Ray &ray, HitRecord &hit) const {
	float denom = glm::dot(ray.direction, n);

	if (denom == 0) {
		hit.t = FLT_MAX;
	}
	else {
		float num = glm::dot(a - ray.origin, n);
		hit.t = num / denom;
		if (hit.t < 0) {
			hit.t = FLT_MAX;
		}
		else {
			hit.interceptPoint = ray.getPoint(hit.t);
			hit.surfaceNormal = n;
		}
	}
}

/**
 * @fn	IPlane::IPlane(const glm::vec3 &point, const glm::vec3 &normal)
 * @brief	Constructor
 * @param	point 	The point.
 * @param	normal	The normal.
 */

IPlane::IPlane(const glm::vec3 &point, const glm::vec3 &normal)
	: IShape(), a(point), n(normalize(normal)) {
}

/**
 * @fn	IPlane::IPlane(const std::vector<glm::vec3> &vertices)
 * @brief	Constructor
 * @param	vertices	The vertices.
 */

IPlane::IPlane(const std::vector<glm::vec3> &vertices)
	: IShape() {
	a = vertices[0];
	n = glm::normalize(glm::cross(vertices[2] - vertices[1], vertices[0] - vertices[1]));
}

/**
 * @fn	IPlane::IPlane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2) : IShape(), a(p1), n(glm::normalize(glm::cross(p2 - p1, p0 - p1)))
 * @brief	Constructor
 * @param	p0	The p 0.
 * @param	p1	The first glm::vec3.
 * @param	p2	The second glm::vec3.
 */

IPlane::IPlane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
	: IShape(), a(p1), n(glm::normalize(glm::cross(p2 - p1, p0 - p1))) {
}

/**
 * @fn	bool IPlane::insidePlane(const glm::vec3 &point) const
 * @brief	Inside plane
 * @param	point	The point.
 * @return	True if it succeeds, false if it fails.
 */

bool IPlane::insidePlane(const glm::vec3 &point) const {
	// If dot product is positive the point is on the "positive" side of the plane
	return glm::dot(point.xyz - a, n) >= 0.0f;
}

/**
 * @fn	void IPlane::findIntersection(const glm::vec3 &p1, const glm::vec3 &p2, float &t) const
 * @brief	Searches for the first intersection
 * @param 		  	p1	The first glm::vec3.
 * @param 		  	p2	The second glm::vec3.
 * @param [in,out]	t 	A float to process.
 */

void IPlane::findIntersection(const glm::vec3 &p1, const glm::vec3 &p2, float &t) const {
	float d1 = glm::dot(p1.xyz - a, n);
	float d2 = glm::dot(p2.xyz - a, n);

	// Find the paramter of the intercept with the plane
	t = d1 / (d1 - d2);
}

/**
 * @fn	IRect::IRect(const glm::vec3 &position, const glm::vec3 &normal, float W, float H)
 * @brief	Constructs a rectangle, given the specific settings.
 * @param	position	The position.
 * @param	normal  	The normal.
 * @param	W			The width.
 * @param	H			The height.
 */

IRect::IRect(const glm::vec3 &position, const glm::vec3 &normal, float W, float H)
	: width(W), height(H), center(position), n(normal), plane(position, normal) {
	W2 = W / 2;
	H2 = H / 2;
}

/**
 * @fn	void IRect::findClosestIntersection(const Ray &ray, HitRecord &hit) const
 * @brief	Searches for the nearest intersection
 * @param 		  	ray	The ray.
 * @param [in,out]	hit	The hit.
 */

void IRect::findClosestIntersection(const Ray &ray, HitRecord &hit) const {
	plane.findClosestIntersection(ray, hit);
	if (hit.t == FLT_MAX) return;

	if (std::abs(n[0]) == 1) {	// yz plane
		if (!inRangeExclusive(hit.interceptPoint[1], center[1] - W2, center[1] + W2) ||
			!inRangeExclusive(hit.interceptPoint[2], center[2] - H2, center[2] + H2))
			hit.t = FLT_MAX;
	} else if (std::abs(n[1]) == 1) {	// xz plane
		if (!inRangeExclusive(hit.interceptPoint[0], center[0] - W2, center[0] + W2) ||
			!inRangeExclusive(hit.interceptPoint[2], center[2] - H2, center[2] + H2))
			hit.t = FLT_MAX;
	} else if (std::abs(n[2]) == 1) {	// xy plane
		if (!inRangeExclusive(hit.interceptPoint[0], center[0] - W2, center[0] + W2) ||
			!inRangeExclusive(hit.interceptPoint[1], center[1] - H2, center[1] + H2))
			hit.t = FLT_MAX;
	}
}

/**
 * @fn	IConvexPolygon::IConvexPolygon(const std::vector<glm::vec3> &vertices)
 * @brief	Constructs a convex polygon, given the vector of vertices.
 * @param	vertices	The vertices.
 */

IConvexPolygon::IConvexPolygon(const std::vector<glm::vec3> &vertices)
	: IPlane(vertices), v(vertices), n(normalFrom3Points(vertices)) {
}

/**
 * @fn	void IConvexPolygon::findClosestIntersection(const Ray &ray, HitRecord &hit) const
 * @brief	Searches for the nearest intersection
 * @param 		  	ray	The ray.
 * @param [in,out]	hit	The hit.
 */

void IConvexPolygon::findClosestIntersection(const Ray &ray, HitRecord &hit) const {
	IPlane::findClosestIntersection(ray, hit);
	hit.surfaceNormal = n;

	if (hit.t < FLT_MAX && !isInside(hit.interceptPoint)) {
		hit.t = FLT_MAX;
	}
}

/**
 * @fn	bool IConvexPolygon::isInside(const glm::vec3 &point) const
 * @brief	Query if 'point' is inside
 * @param	point	The point.
 * @return	true iff inside.
 */

bool IConvexPolygon::isInside(const glm::vec3 &point) const {
	bool insideOnFrontSide = true;
	bool insideOnBackSide = true;

	for (unsigned int i = 0; i < v.size(); i++) {
		const glm::vec3 pointRay = point - v[i];
		const glm::vec3 edgeRay = v[(i + 1) % v.size()] - v[i];

		if (glm::dot(glm::cross(edgeRay, pointRay), n) < 0) {
			insideOnFrontSide = false;
		}
		if (glm::dot(glm::cross(edgeRay, pointRay), n) > 0) {
			insideOnBackSide = false;
		}
	}

	return insideOnFrontSide || insideOnBackSide;
}

/**
 * @fn	IQuadricSurface::IQuadricSurface(const QuadricParameters &params, const glm::vec3 &position)
 * @brief	Constructs an implicit representation of a QuadricSurface.
 * @param	params  	Options for controlling the operation.
 * @param	position	The position.
 */

IQuadricSurface::IQuadricSurface(const QuadricParameters &params,
								const glm::vec3 &position)
		: IShape(), qParams(params), center(position) {
	twoA = 2.0f * qParams.A;
	twoB = 2.0f * qParams.B;
	twoC = 2.0f * qParams.C;
}

/**
 * @fn	IQuadricSurface::IQuadricSurface(const std::vector<float> &params, const glm::vec3 &position)
 * @brief	Constructs an implicit representation of a QuadricSurface.
 * @param	params  	Quadric parameters.
 * @param	position	The position of the quadric.
 */

IQuadricSurface::IQuadricSurface(const std::vector<float> &params,
								const glm::vec3 &position) 
		: IQuadricSurface(QuadricParameters(params), position) {
}

/**
 * @fn	IQuadricSurface::IQuadricSurface(const glm::vec3 &position)
 * @brief	Constructs an implicit representation of a QuadricSurface.
 * @param	position	The position of the quadric.
 */

IQuadricSurface::IQuadricSurface(const glm::vec3 &position)
	: IQuadricSurface(QuadricParameters(), position) {
}

/**
 * @fn	void IQuadricSurface::computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const
 * @brief	Calculates the aq bq cq
 * @param 		  	ray	The ray.
 * @param [in,out]	Aq 	The aq.
 * @param [in,out]	Bq 	The bq.
 * @param [in,out]	Cq 	The cq.
 */

void IQuadricSurface::computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const {
	glm::vec3 Ro = ray.origin - center;
	const glm::vec3 &Rd = ray.direction;
	const float &A = qParams.A;
	const float &B = qParams.B;
	const float &C = qParams.C;
	const float &D = qParams.D;
	const float &E = qParams.E;
	const float &F = qParams.F;
	const float &G = qParams.G;
	const float &H = qParams.H;
	const float &I = qParams.I;
	const float &J = qParams.J;
	Aq = A * (Rd.x*Rd.x) +
		B * (Rd.y*Rd.y) +
		C * (Rd.z*Rd.z) +
		D * (Rd.x * Rd.y) +
		E * (Rd.x * Rd.z) +
		F * (Rd.y * Rd.z);

	Bq = twoA * Ro.x*Rd.x +
		twoB * Ro.y*Rd.y +
		twoC * Ro.z*Rd.z +
		D * (Ro.x * Rd.y + Ro.y * Rd.x) +
		E * (Ro.x * Rd.z + Ro.z * Rd.x) +
		F * (Ro.y * Rd.z + Ro.z * Rd.y) +
		G * Rd.x + H * Rd.y + I * Rd.z;

	Cq = A * (Ro.x * Ro.x) +
		B * (Ro.y * Ro.y) +
		C * (Ro.z * Ro.z) +
		D * (Ro.x * Ro.y) +
		E * (Ro.x * Ro.z) +
		F * (Ro.y * Ro.z) +
		G * Ro.x +
		H * Ro.y +
		I * Ro.z + J;
}

/**
 * @fn	int IQuadricSurface::findIntersections(const Ray &ray, HitRecord hits[2]) const
 * @brief	Searches for the first intersections
 * @param	ray 	The ray.
 * @param	hits	The hits.
 * @return	The found intersections.
 */

int IQuadricSurface::findIntersections(const Ray &ray, HitRecord hits[2]) const {
	float Aq, Bq, Cq;
	computeAqBqCq(ray, Aq, Bq, Cq);
	float roots[2];

	int numRoots = quadratic(Aq, Bq, Cq, roots);
	int numIntersections = 0;

	for (int i = 0; i < numRoots; i++) {
		if (roots[i] > 0) {
			const float &t = roots[i];
			hits[numIntersections].t = t;
			hits[numIntersections].interceptPoint = ray.origin + t * ray.direction;
			const glm::vec3 &intercept = hits[numIntersections].interceptPoint;
			hits[numIntersections].surfaceNormal = normal(intercept);
			numIntersections++;
		}
	}

	return numIntersections;
}

/**
 * @fn	void IQuadricSurface::findClosestIntersection(const Ray &ray, HitRecord &hit) const
 * @brief	Searches for the nearest intersection
 * @param 		  	ray	The ray.
 * @param [in,out]	hit	The hit.
 */

void IQuadricSurface::findClosestIntersection(const Ray &ray, HitRecord &hit) const {
	static HitRecord hits[2];
	hit.t = FLT_MAX;

	int numIntercepts = findIntersections(ray, hits);
	if (numIntercepts == 1 && hits[0].t > 0) {
		hit.t = hits[0].t;
		hit.interceptPoint = hits[0].interceptPoint;
		hit.surfaceNormal = normal(hit.interceptPoint);
	} else if (numIntercepts == 2) {
		if (hits[0].t > 0) {
			hit.t = hits[0].t;
			hit.interceptPoint = hits[0].interceptPoint;
			hit.surfaceNormal = normal(hit.interceptPoint);
		} else if (hits[1].t > 0) {
			hit.t = hits[1].t;
			hit.interceptPoint = hits[1].interceptPoint;
			hit.surfaceNormal = normal(hit.interceptPoint);
		}
	}
}

/**
 * @fn	glm::vec3 IQuadricSurface::normal(const glm::vec3 &P) const
 * @brief	Normals the given p
 * @param	P	A glm::vec3 to process.
 * @return	A glm::vec3.
 */

glm::vec3 IQuadricSurface::normal(const glm::vec3 &P) const {
	const float &A = qParams.A;
	const float &B = qParams.B;
	const float &C = qParams.C;
	const float &D = qParams.D;
	const float &E = qParams.E;
	const float &F = qParams.F;
	const float &G = qParams.G;
	const float &H = qParams.H;
	const float &I = qParams.I;
	const float &J = qParams.J;
	glm::vec3 pt = P - center;
	glm::vec3 normal(twoA * pt.x + D * pt.y + E * pt.z + G,
					twoB * pt.y + D * pt.x + F * pt.z + H,
					twoC * pt.z + E * pt.x + F * pt.y + I);
	return glm::normalize(normal);
}

/**
 * @fn	ICylinder::ICylinder(const glm::vec3 &pos, float R, float L, const QuadricParameters &qParams)
 * @brief	Constructs an implicit representation of a cylinder.
 * @param	pos	   	The position.
 * @param	R	   	Radius.
 * @param	L	   	Length of cylinder.
 * @param	qParams	Quadric parameters.
 */

ICylinder::ICylinder(const glm::vec3 &pos, float R, float L,
					const QuadricParameters &qParams)
	: IQuadricSurface(qParams, pos), radius(R), length(L) {
}

/**
 * @fn	void ICylinder::computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const
 * @brief	Calculates the aq bq cq
 * @param 		  	ray	The ray.
 * @param [in,out]	Aq 	The aq.
 * @param [in,out]	Bq 	The bq.
 * @param [in,out]	Cq 	The cq.
 */

void ICylinder::computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const {
	const float &A = qParams.A;
	const float &B = qParams.B;
	const float &C = qParams.C;
	const float &D = qParams.D;
	const float &E = qParams.E;
	const float &F = qParams.F;
	const float &G = qParams.G;
	const float &H = qParams.H;
	const float &I = qParams.I;
	const float &J = qParams.J;
	glm::vec3 Ro = ray.origin - center;
	const glm::vec3 &Rd = ray.direction;

	Aq = A * (Rd.x*Rd.x) +
		B * (Rd.y*Rd.y) +
		C * (Rd.z*Rd.z);
	//D * (Rd.x * Rd.y) +
	//E * (Rd.x * Rd.z) +
	//F * (Rd.y * Rd.z);

	Bq = twoA * Ro.x*Rd.x +
		twoB * Ro.y*Rd.y +
		twoC * Ro.z*Rd.z;
	//D * (Ro.x * Rd.y + Ro.y * Rd.x) +
	//E * (Ro.x * Rd.z + Ro.z * Rd.x) +
	//F * (Ro.y * Rd.z + Ro.z * Rd.y) +
	//G * Rd.x + H * Rd.y + I * Rd.z;

	Cq = A * (Ro.x * Ro.x) +
		B * (Ro.y * Ro.y) +
		C * (Ro.z * Ro.z) +
		//D * (Ro.x * Ro.y) +
		//E * (Ro.x * Ro.z) +
		//F * (Ro.y * Ro.z) +
		//G * Ro.x +
		//H * Ro.y +
		I * Ro.z + J;
}

/**
 * @fn	ICylinderY::ICylinderY(const glm::vec3 &pos, float rad, float len) : ICylinder(pos, rad, len, QuadricParameters::cylinderYQParams(rad))
 * @brief	Constructor
 * @param	pos	The position.
 * @param	rad	The radians.
 * @param	len	The length.
 */

ICylinderY::ICylinderY(const glm::vec3 &pos, float rad, float len)
	: ICylinder(pos, rad, len, QuadricParameters::cylinderYQParams(rad)) {
}

/**
 * @fn	void ICylinderY::findClosestIntersection(const Ray &ray, HitRecord &hit) const
 * @brief	Searches for the nearest intersection
 * @param 		  	ray	The ray.
 * @param [in,out]	hit	The hit.
 */

void ICylinderY::findClosestIntersection(const Ray &ray, HitRecord &hit) const {
	const glm::vec3 &rayOrigin = ray.origin;
	const glm::vec3 &rayDirection = ray.direction;
	static HitRecord hits[2];
	int numHits = ICylinder::findIntersections(ray, hits);
	for (int i = 0; i < numHits; i++) {
		if (hits[i].interceptPoint.y < center.y + length / 2 &&
			hits[i].interceptPoint.y > center.y - length / 2) {
				hit = hits[i];
				return;
		}
	}
	hit.t = FLT_MAX;
}

IClosedCylinderY::IClosedCylinderY(const glm::vec3 &pos, float rad, float len)
	: ICylinderY(pos, rad, len), top(pos+glm::vec3(0, len/2, 0), glm::vec3(0,1,0) ,rad), bottom(pos+glm::vec3(0, -len/2,0), glm::vec3(0, -1,0) ,rad) {
	
}

void IClosedCylinderY::findClosestIntersection(const Ray &ray, HitRecord &hit) const {
	const glm::vec3 &rayOrigin = ray.origin;
	const glm::vec3 &rayDirection = ray.direction;
	static HitRecord hits[2];
	int numHits = ICylinder::findIntersections(ray, hits);
	for (int i = 0; i < numHits; i++) {
		if (hits[i].interceptPoint.y < center.y + length / 2 &&
		hits[i].interceptPoint.y > center.y - length / 2 && 
		(hits[i].interceptPoint.x < top.center.x + top.radius ||
		hits[i].interceptPoint.x > top.center.x - top.radius)) {
			hit = hits[i];
			return;
		}
	}
	hit.t = FLT_MAX;
}

//use quadric surfaces to implement cone

/**
* @fn	void ICylinderY::getTexCoords(const glm::vec3 &pt, float &u, float &v) const
* @brief	Gets tex coordinates
* @param 		  	pt	The point.
* @param [in,out]	u 	Tex coordinate u.
* @param [in,out]	v 	Tex coordinate v.
*/

// needs to be adapted to allow for cylinders NOT centered origin
void ICylinderY::getTexCoords(const glm::vec3 &pt, float &u, float &v) const {
	float angle = normalizeRadians(std::atan2(pt.z, pt.x));
	float bottom = center.y - length / 2.0f;
	u = angle / M_2PI;
	v = (pt.y - bottom) / length;
}

/**
 * @fn	ITriangle::ITriangle(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C)
 * @brief	Constructs and implicit representation of a triangle, given three vertices
 * 			presented in counterclockwise order.
 * @param	A	First vertex.
 * @param	B	Second vertex.
 * @param	C	Third vertex.
 */

ITriangle::ITriangle(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C)
	: IShape(), a(A), b(B), c(C), plane(IPlane(A, B, C)) {
}

/**
 * @fn	bool ITriangle::inside(const glm::vec3 &pt) const
 * @brief	Insides the given point
 * @param	pt	The point.
 * @return	true iff the point is inside the triangle.
 */

bool ITriangle::inside(const glm::vec3 &pt) const { // equation 2.35
	glm::vec3 n = glm::cross(b - a, c - a);
	float n2 = std::pow(glm::length(n), 2.0f);
	glm::vec3 na = glm::cross(c - b, pt - b);
	glm::vec3 nb = glm::cross(a - c, pt - c);
	glm::vec3 nc = glm::cross(b - a, pt - a);
	float alpha = glm::dot(n, na) / n2;
	float beta = glm::dot(n, nb) / n2;
	float gamma = glm::dot(n, nc) / n2;
	return inRangeExclusive(beta, 0, 1) && inRangeExclusive(gamma, 0, 1) && inRangeExclusive(alpha, 0, 1);
}

/**
 * @fn	void ITriangle::findClosestIntersection(const Ray &ray, HitRecord &hit) const
 * @brief	Searches for the nearest intersection
 * @param 		  	ray	The ray.
 * @param [in,out]	hit	Hit record.
 */

void ITriangle::findClosestIntersection(const Ray &ray, HitRecord &hit) const {
	plane.findClosestIntersection(ray, hit);
	hit.surfaceNormal = plane.n;
	if (!inside(hit.interceptPoint)) {
		hit.t = FLT_MAX;
	}
}

/**
 * @fn	IEllipsoid::IEllipsoid(const glm::vec3 &position, const glm::vec3 &sz) : IQuadricSurface(QuadricParameters::ellipoidParameters(sz), position)
 * @brief	Constructs an implicit representation of an ellipsoid.
 * @param	position	The center of ellipsoid.
 * @param	sz			The size of ellipsoid.
 */

IEllipsoid::IEllipsoid(const glm::vec3 &position, const glm::vec3 &sz)
	: IQuadricSurface(QuadricParameters::ellipsoidQParams(sz), position) {
}

/**
 * @fn	void IEllipsoid::computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const
 * @brief	Calculates the aq bq cq, given a particular ray.
 * @param 		  	ray	The ray.
 * @param [in,out]	Aq 	Aq.
 * @param [in,out]	Bq 	Bq.
 * @param [in,out]	Cq 	Cq.
 */

void IEllipsoid::computeAqBqCq(const Ray &ray, float &Aq, float &Bq, float &Cq) const {
	const float &A = qParams.A;
	const float &B = qParams.B;
	const float &C = qParams.C;
	const float &D = qParams.D;
	const float &E = qParams.E;
	const float &F = qParams.F;
	const float &G = qParams.G;
	const float &H = qParams.H;
	const float &I = qParams.I;
	const float &J = qParams.J;
	glm::vec3 Ro = ray.origin - center;
	const glm::vec3 &Rd = ray.direction;

	Aq = A * (Rd.x*Rd.x) +
		B * (Rd.y*Rd.y) +
		C * (Rd.z*Rd.z);
	//D * (Rd.x * Rd.y) +
	//E * (Rd.x * Rd.z) +
	//F * (Rd.y * Rd.z);

	Bq = 2 * A * Ro.x*Rd.x +
		2 * B * Ro.y*Rd.y +
		2 * C * Ro.z*Rd.z;
	//D * (Ro.x * Rd.y + Ro.y * Rd.x) +
	//E * (Ro.x * Rd.z + Ro.z * Rd.x) +
	//F * (Ro.y * Rd.z + Ro.z * Rd.y) +
	//G * Rd.x + H * Rd.y + I * Rd.z;

	Cq = A * (Ro.x * Ro.x) +
		B * (Ro.y * Ro.y) +
		C * (Ro.z * Ro.z) +
		//D * (Ro.x * Ro.y) +
		//E * (Ro.x * Ro.z) +
		//F * (Ro.y * Ro.z) +
		//G * Ro.x +
		//H * Ro.y +
		I * Ro.z + J;
}