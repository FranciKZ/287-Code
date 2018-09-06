#include "ColorAndMaterials.h"

/**
 * @fn	color brighten(const color &C)
 * @brief	Returns a brighter version of the given color.
 * @param	C	The color to modulate.
 * @return	The brighten color.
 */

color brighten(const color &C) {
	return C;
}

/**
* @fn	color darken(const color &C)
* @brief	Returns a darker version of the given color.
* @param	C	The color to modulate.
* @return	The darkened color.
*/
color darken(const color &C) {
	return C;
}

/**
 * @fn	float brightness(const color &C)
 * @brief	Computes the brightness of a color [0,1] - length of the <R, G, B> vector.
 * @param	C	A color to process.
 * @return	C's brightness
 */

float brightness(const color &C) {
	return glm::length(C) / std::sqrt(3.0f);
}

/**
 * @fn	Material::Material(const color &amb, const color &diff, const color &spec, float shininess)
 * @brief	Construct a Materials based on the basic color and shinieness values.
 * @param	amb			Ambient
 * @param	diff		Diffuse
 * @param	spec		Specular
 * @param	shininess	Shinieness
 */

Material::Material(const color &amb, 
					const color &diff, 
					const color &spec, float S) {
	ambient = amb;
	diffuse = diff;
	specular = spec;
	shininess = S;
	alpha = 1.0;
}

/**
 * @fn	Material::Material(const std::vector<float> &C)
 * @brief	Construct a Materials based on the basic color and shinieness values.
 * @param	C			Vector holding 10 values: ambient, diffuse, specular, shinieness
 */

Material::Material(const std::vector<float> &C) :
	Material(color(C[0], C[1], C[2]),
		color(C[3], C[4], C[5]),
		color(C[6], C[7], C[8]),
		C[9]) {
}

/**
 * @fn	Material::Material(const color &oneColor)
 * @brief	Constructs a material that has ambient values only. Diffuse and specular are black.
 * @param	oneColor	Ambient values
 */

Material::Material(const color &oneColor) {
	ambient = oneColor;
	diffuse = specular = black;
	shininess = 0.0f;
	alpha = 1.0f;
}

/**
 * @fn	Material operator*(float w) const
 * @brief	Multiply a Material by a scalar value.
 * @param	w	Weight of multiplication.
 * @return	The Material resulting from multiplying the given Material by the given weight.
 */

Material Material::operator *(float w) const {
	Material result = *this;
	result.alpha *= w;
	result.ambient *= w;
	result.diffuse *= w;
	result.specular *= w;
	return result;
}

/**
 * @fn	Material &Material::operator+=(const Material &mat)
 * @brief	Add in a second material.
 * @param	mat	The second Material.
 * @return	The revised Material.
 */

Material &Material::operator +=(const Material &mat) {
	Material result = *this;
	alpha += mat.alpha;
	ambient += mat.ambient;
	diffuse += mat.diffuse;
	specular += mat.specular;
	return *this;
}

/**
 * @fn	Material Material::operator+(const Material &mat) const
 * @brief	Adds two Materials
 * @param	mat	The second Material.
 * @return	The sum of two Materials.
 */

Material Material::operator +(const Material &mat) const {
	Material result = *this;
	result.alpha += mat.alpha;
	result.ambient += mat.ambient;
	result.diffuse += mat.diffuse;
	result.specular += mat.specular;
	return result;
}

/**
 * @fn	Material Material::operator-(const Material &mat) const
 * @brief	Subtracts two materials
 * @param	mat	The second Material.
 * @return	The subtraction of two Materials.
 */

Material Material::operator -(const Material &mat) const {
	Material result = *this;
	result.alpha -= mat.alpha;
	result.ambient -= mat.ambient;
	result.diffuse -= mat.diffuse;
	result.specular -= mat.specular;
	return result;
}

/**
 * @fn	Material Material::makeTransparent(float alpha, const color &C)
 * @brief	Makes a transparent version of a given color.
 * @param	alpha	The alpha value.
 * @param	C	 	The underlying color.
 * @return	The transparent Material.
 */

Material Material::makeTransparent(float alpha, const color &C) {
	Material result(C);
	result.alpha = alpha;
	return result;
}

/**
 * @fn	Material operator*(float w, const Material &mat)
 * @brief	Multiply a Material and a scalar.
 * @param	w  	The scalar multiplicand
 * @param	mat	Material
 * @return	The original material multiplied by given weight.
 */

Material operator *(float w, const Material &mat) {
	return mat * w;
}

