#pragma once
#include "stdafx.h"
// data class to hold lighting information, for ease of passing around the system

class PointLight
{
	glm::mat4 position;
	glm::mat4 color;
	float ambientLevel;
	float diffuseLevel;
	float specularLevel;
	float specularPower;
	glm::vec4 attenuationRadius;
public:
	PointLight() {};
	PointLight(const glm::mat4& position, const glm::mat4& color, float ambientLevel,
		float diffuseLevel, float specularLevel, float specularPower, glm::vec4 attenuationRadius) :
		position(position), color(color), ambientLevel(ambientLevel), diffuseLevel(diffuseLevel),
		specularLevel(specularLevel), specularPower(specularPower), attenuationRadius(attenuationRadius) {}
	// getters
	const glm::mat4& GetPosition() const { return position;  }
	const glm::mat4& GetColor() const { return color; }
	float GetAmbientLevel() const { return ambientLevel; }
	float GetDiffuseLevel() const { return diffuseLevel; }
	float GetSpecularLevel() const { return specularLevel; }
	float GetSpecularPower() const { return specularPower; }
	const glm::vec4& GetAttenuationRadius() const { return attenuationRadius; }
	// setters
	void SetPosition(const glm::mat4 &newPos) { position = newPos; }
	void SetColor(const glm::mat4 & newColor) { color = newColor; }
	void SetAmbientLevel(float newAmbient) { ambientLevel = newAmbient; }
	void SetDiffuseLevel(float newDiffuse) { diffuseLevel = newDiffuse; }
	void SetSpecularLevel(float newSpecular) { specularLevel = newSpecular; }
	void SetSpecularPower(float newPower) { specularPower = newPower; }
	void SetAttenuationRadius(const glm::vec4& newAttenuation) { attenuationRadius = newAttenuation; }
};