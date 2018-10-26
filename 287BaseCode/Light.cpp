#include "Light.h"

/**
 * @fn	color ambientColor(const color &mat, const color &light)
 * @brief	Computes the ambient color produced by a single light at a single point.
 * @param	mat  	Ambient material property.
 * @param	light	Light's ambient color.
 * @return	Ambient color.
  */

color ambientColor(const color &mat, const color &light) {
	return glm::vec3(light.x * mat.x, light.y * mat.y, light.z * mat.z);
}

/**
 * @fn	color diffuseColor(const color &mat, const color &light, const glm::vec3 &l, const glm::vec3 &n)
 * @brief	Computes diffuse color produce by a single light at a single point.
 * @param	mat		 	Material.
 * @param	light	 	The light.
 * @param	l		 	Light vector.
 * @param	n		 	Normal vector.
 * @return	Diffuse color.
 */

color diffuseColor(const color &mat, const color &light,
					const glm::vec3 &l, const glm::vec3 &n) {
	return ambientColor(mat, light) * (glm::dot(l, n));
}

/**
 * @fn	color specularColor(const color &mat, const color &light, float shininess, const glm::vec3 &r, const glm::vec3 &v)
 * @brief	Computes specular color produce by a single light at a single point.
 * @param	mat		 	Material.
 * @param	light	 	The light's color.
 * @param	shininess	Material shininess.
 * @param	r		 	Reflection vector.
 * @param	v		 	Viewing vector.
 * @return	Specular color.
 */
// clamp glm::dot(r, v) to[0,1]???????
color specularColor(const color &mat, const color &light,
					float shininess,
					const glm::vec3 &r, const glm::vec3 &v) {
	float rv = glm::clamp(glm::dot(r, v), 0.0f, 1.0f);
	return ambientColor(mat, light) * pow(rv, shininess);
}

/**
 * @fn	color totalColor(const Material &mat, const LightColor &lightColor, const glm::vec3 &viewingDir, const glm::vec3 &normal, const glm::vec3 &lightPos, const glm::vec3 &intersectionPt, bool attenuationOn, const LightAttenuationParameters &ATparams)
 * @brief	Color produced by a single light at a single point.
 * @param	mat			  	Material.
 * @param	lightColor	  	The light's color.
 * @param	v	  			The v vector.
 * @param	normal		  	Normal vector.
 * @param	lightPos	  	Light position.
 * @param	intersectionPt	(x,y,z) of intersection point.
 * @param	attenuationOn 	true if attenuation is on.
 * @param	ATparams	  	Attenuation parameters.
 * @return	Color produced by a single light at a single point.
 */
 
// add suppport for attentuation
color totalColor(const Material &mat, const LightColor &lightColor,
				const glm::vec3 &v, const glm::vec3 &n,
				const glm::vec3 &lightPos, const glm::vec3 &intersectionPt,
				bool attenuationOn, 
				const LightAttenuationParameters &ATparams) {
	if (DEBUG_PIXEL) {
		std::cout << std::endl;
	}
	glm::vec3 l = glm::normalize(lightPos - intersectionPt);
	glm::vec3 r = 2.0f * glm::dot(l, n) * n - l;
	color amb = ambientColor(mat.ambient, lightColor.ambient);
	color diff = diffuseColor(mat.diffuse, lightColor.diffuse, l, n);
	color spec = specularColor(mat.specular, lightColor.specular, mat.shininess, r, v);
	// computer attentuation factor and multiply dif f and spec
	if (attenuationOn) {
		float d = glm::distance(intersectionPt, lightPos);
		float attenFactor = 1.0f / (ATparams.constant + (ATparams.linear * d) + ATparams.quadratic * pow(d, 2));
		return amb + attenFactor * (diff + spec);
	}
	return amb + diff + spec;
}

/**
 * @fn	color PositionalLight::illuminate(const HitRecord &hit, const glm::vec3 &viewingDir, const Frame &eyeFrame, bool inShadow) const
 * @brief	Computes the color this light produces in raytracing applications.
 * @param	hit					The surface properties of the intercept point.
 * @param	v					The v vector
 * @param	eyeFrame			The coordinate frame of the camera.
 * @param	inShadow			true if the point is in a shadow.
 * @return	The color produced at the intercept point, given this light.
 */

color PositionalLight::illuminate(const glm::vec3 &interceptWorldCoords,
									const glm::vec3 &normal,
									const Material &material,
									const Frame &eyeFrame, bool inShadow) const {
	color ambient = glm::clamp(ambientColor(material.ambient, this->lightColorComponents.ambient), 0.0f, 1.0f);
	color total = glm::clamp(totalColor(material, this->lightColorComponents,
							glm::normalize(glm::vec3(eyeFrame.origin - interceptWorldCoords)),
							normal, this->lightPosition,
							interceptWorldCoords, this->attenuationIsTurnedOn, this->attenuationParams), 0.0f, 1.0f);
	if (!isOn) {
		return black;
	}
	else if (inShadow) {
		return ambient;
	}
	else {		
		return total;
	}
}

/**
 * @fn	color SpotLight::illuminate(const HitRecord &hit, const glm::vec3 &viewingDir, const Frame &eyeFrame, bool inShadow) const
 * @brief	Computes the color this light produces in raytracing applications.
 * @param	hit					The surface properties of the intercept point.
 * @param	v					The v vector
 * @param	eyeFrame			The coordinate frame of the camera.
 * @param	inShadow			true if the point is in a shadow.
 * @return	The color produced at the intercept point, given this light.
 */

// dot product of spotlight direction and pointing vector of light pos, cos(theta), normalize, 
color SpotLight::illuminate(const glm::vec3 &interceptWorldCoords,
							const glm::vec3 &normal,
							const Material &material,
							const Frame &eyeFrame, bool inShadow) const {
	// position -> intercept
	glm::vec3 pointV = glm::normalize(pointingVector(lightPosition, interceptWorldCoords));
	bool inCone = deg2rad(glm::dot(pointV, spotDirection)) < 
		fov;
	if (!isOn || !inCone) {
		return black;
	}
	else {
		return PositionalLight::illuminate(interceptWorldCoords, normal, material, eyeFrame, inShadow);
	}
}

/**
* @fn	ostream &operator << (std::ostream &os, const LightAttenuationParameters &at)
* @brief	Output stream for light attenuation parameters.
* @param	os		Output stream.
* @param	at		Attenuation parameters.
* @return	The output stream.
*/

std::ostream &operator << (std::ostream &os, const LightAttenuationParameters &at) {
	os << glm::vec3(at.constant, at.linear, at.quadratic) << std::endl;
	return os;
}

/**
* @fn	ostream &operator << (std::ostream &os, const PositionalLight &pl)
* @brief	Output stream for light attenuation parameters.
* @param	os		Output stream.
* @param	pl		Positional light.
* @return	The output stream.
*/

std::ostream &operator << (std::ostream &os, const PositionalLight &pl) {
	os << (pl.isOn ? "ON" : "OFF") << std::endl;
	os << (pl.isTiedToWorld? "WORLD" : "CAMERA") << std::endl;
	os << " position " << pl.lightPosition << std::endl;
	os << " ambient " << pl.lightColorComponents.ambient << std::endl;
	os << " diffuse " << pl.lightColorComponents.diffuse << std::endl;
	os << " specular " << pl.lightColorComponents.specular << std::endl;
	os << "Attenuation: " << (pl.attenuationIsTurnedOn ? "ON" : "OFF")
		<< " " << pl.attenuationParams << std::endl;
	return os;
}

/**
* @fn	ostream &operator << (std::ostream &os, const SpotLight &sl)
* @brief	Output stream for light attenuation parameters.
* @param	os		Output stream.
* @param	sl		Spotlight.
* @return	The output stream.
*/

std::ostream &operator << (std::ostream &os, const SpotLight &sl) {
	PositionalLight pl = (sl);
	os << pl;
	os << " FOV " << sl.fov << std::endl;
	return os;
}