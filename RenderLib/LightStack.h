#pragma once

#include "PointLight.h"
#include "LightNode.h"

class LightStack
{
	std::vector<SingleLight> lights;
public:
	void Clear() { lights.clear(); }
	void Push(LightNode* ln, const glm::mat4& curTransform ) 
	{ 
		SingleLight l = ln->light;
		l.position = curTransform * glm::vec4(l.position,1);
		lights.push_back(l);
	}
	void Pop() { lights.pop_back(); }
	PointLight GetLights()
	{
		// construct a PointLight object from the first four lights.
		glm::mat4 lightPos;
		glm::mat4 lightColor;
		glm::vec4 lightRadius;

		float ambientLevel = 0;
		float diffuseLevel = 0;
		float specularLevel = 0;
		float specularPower = 0;

		for ( int i = 0; i < 4; i++ )
		{
			if ( i < lights.size() )
			{
				// take the 'material' parameters from light 0, if any lights
				ambientLevel = lights[0].ambientLevel;
				diffuseLevel = lights[0].diffuseLevel;
				specularLevel = lights[0].specularLevel;
				specularPower = lights[0].specularPower;
				// take color, position and radius from this light
				lightColor[i] = lights[i].color;
				lightPos[i] = glm::vec4(lights[i].position,1);
				lightRadius[i] = lights[i].radius;
			}
		}
		PointLight l(lightPos, lightColor, ambientLevel, diffuseLevel, specularLevel, specularPower, lightRadius );

	return l;
	}
};