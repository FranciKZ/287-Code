#pragma once
#include <vector>
#include "defs.h"
#include "HitRecord.h"

/**
 * @struct	LightAttenuationParameters
 * @brief	A light attenuation parameters.
 */

struct LightAttenuationParameters {
	float constant, linear, quadratic;	//!< Parameters controlling attenuation.
	LightAttenuationParameters(float C = 1, float L = 0, float Q = 0) {
		constant = C;
		linear = L;
		quadratic = Q;
	}
	float factor(float distance) const {
		return 1.0f / (constant + linear * distance + quadratic * distance * distance);
	}
	friend std::ostream &operator << (std::ostream &os, const LightAttenuationParameters &at);
};

const LightAttenuationParameters noEffectAttenuationParams(1, 0, 0);
const LightAttenuationParameters testATParams(1, 2, 0);
/**
 * @struct	LightColor
 * @brief	Represents the colors of each light component.
 */

struct LightColor {
	color ambient, diffuse, specular;	//!< The three light components.
	LightColor(const color &amb, const color &diff, const color &spec)
		: ambient(amb), diffuse(diff), specular(spec) {
	}
	LightColor(const color &oneColor)
		: ambient(oneColor), diffuse(oneColor), specular(oneColor) {
	}
	LightColor(const std::vector<float> &C)
		: LightColor(color(C[0], C[1], C[2]),
			color(C[3], C[4], C[5]),
			color(C[6], C[7], C[8])) {
	}
};

/**
 * @struct	LightSource
 * @brief	A generic light source.
 */

struct LightSource {
	bool isOn;			//!< True if the light is active; otherwise, has no effect.
	LightSource() {
		isOn = true;
	}
	virtual color illuminate(const glm::vec3 &interceptWorldCoords,
								const glm::vec3 &normal, 
								const Material &material,
								const Frame &eyeFrame, bool inShadow) const = 0;
	};

/**
 * @struct	PositionalLight
 * @brief	Represents a simple positional light source.
 */

struct PositionalLight : public LightSource {
	glm::vec3 lightPosition;	//!< The position of the light.
	bool attenuationIsTurnedOn;	//!< true if attenuation is active.
	bool isTiedToWorld;			//!< true if the position is in world (or eye) coordinates.
	LightAttenuationParameters attenuationParams;
	LightColor lightColorComponents;

	PositionalLight(const glm::vec3 &position, const LightColor &lightColor)
		: LightSource(), lightColorComponents(lightColor), lightPosition(position) {
		attenuationIsTurnedOn = false;
		isTiedToWorld = true;
		attenuationParams = noEffectAttenuationParams;
	}
	void setAttenuation(bool isOn) {
		attenuationIsTurnedOn = isOn;
	}
	void setAttenuationParams(const LightAttenuationParameters &params) {
		attenuationParams = params;
	}
	virtual color illuminate(const glm::vec3 &interceptWorldCoords,
							const glm::vec3 &normal,
							const Material &material,
							const Frame &eyeFrame, bool inShadow) const;
	friend std::ostream &operator << (std::ostream &os, const PositionalLight &pl);
};

/**
 * @struct	SpotLight
 * @brief	A spot light.
 */

struct SpotLight : public PositionalLight {
	float fov;					//!< Field of view of the light.
	glm::vec3 spotDirection;	//!< Direction of spotlight.
	SpotLight(const glm::vec3 &position, const glm::vec3 &dir,
		float angleInRadians, const LightColor &lightColor)
		: PositionalLight(position, lightColor), spotDirection(dir),
		fov(angleInRadians) {
	}
	virtual color illuminate(const glm::vec3 &interceptWorldCoords,
							const glm::vec3 &normal,
							const Material &material,
							const Frame &eyeFrame, bool inShadow) const;
	friend std::ostream &operator << (std::ostream &os, const SpotLight &pl);
};

const LightColor pureWhiteLight(std::vector<float>{1, 1, 1, 1, 1, 1, 1, 1, 1});
const LightColor standardWhiteLight(std::vector<float>{0.2f, 0.2f, 0.2f, 1, 1, 1, 1, 1, 1});
const LightColor testLight(std::vector<float>{0.3f, 0.2f, 0.1f, 1, 1, 1, 0.5f, 0.6f, 0.7f});

color ambientColor(const color &matAmbient, const color &lightAmbient);
color diffuseColor(const color &matDiffuse, const color &lightDiffuse,
					const glm::vec3 &l, const glm::vec3 &n);
color specularColor(const color &mat, const color &light,
					float shininess,
					const glm::vec3 &r, const glm::vec3 &v);
color totalColor(const Material &mat, const LightColor &lightColor,
					const glm::vec3 &viewingDir, const glm::vec3 &normal,
					const glm::vec3 &lightPos, const glm::vec3 &intersectionPt,
					bool attenuationOn = false,
					const LightAttenuationParameters &ATparams = noEffectAttenuationParams);

typedef LightSource *LightSourcePtr;
typedef PositionalLight *PositionalLightPtr;
typedef SpotLight *SpotLightPtr;
