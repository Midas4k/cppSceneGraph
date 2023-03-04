#pragma once

#include "PointLight.h"

class LightStack
{
	std::vector<SingleLight> lights;
public:
	void Push(const SingleLight& l, const glm::mat4& curTransform ) 
	{ 
		lights.push_back(l);
		// transform the position
		lights[lights.size() - 1].position *= curTransform;
	}
	void Pop() { lights.pop_back(); }
	PointLight GetLights()
	{
		// construct a PointLight object from the first four lights.
		// to do
	}
};