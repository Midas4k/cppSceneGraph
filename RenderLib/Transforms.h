#pragma once
#include "stdafx.h"
// data class to encapsulate model, view, projection for ease of passing around the system

class Transforms
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
public:
	Transforms() {};
	Transforms(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj) : model(model), view(view), proj(proj) {}
	// getters
	const glm::mat4& GetModel() const { return model; }
	const glm::mat4& GetView() const { return view; }
	const glm::mat4& GetProj() const { return proj; }
	// setters
	void SetModel(const glm::mat4 &newModel) { model = newModel; }
	void SetView(const glm::mat4 &newView) { view = newView; }
	void SetProj(const glm::mat4 &newProj) { proj = newProj; }
};