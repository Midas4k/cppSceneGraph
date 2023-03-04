#pragma once
#include "stdafx.h"
#include "GeometryHelper.h"

class QuadGeometry : public GeometryHelper
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;

	void CreateGeometry(float uvScale)
	{
		glm::vec3 vertexPositions[] = {
			glm::vec3(-0.5f, 0.0f, -0.5f),
			glm::vec3(-0.5f, 0.0f, 0.5f),
			glm::vec3(0.5f, 0.0f, 0.5f),
			glm::vec3(0.5f, 0.0f, -0.5f),
		};

		glm::vec2 vertexUVs[] =
		{
			glm::vec2(0,0),
			glm::vec2(0,uvScale),
			glm::vec2(uvScale, uvScale),
			glm::vec2(uvScale,0),

		};

		glm::vec3 faceNormal  = glm::vec3(0,1,0);
		glm::vec3 faceTangent = glm::vec3(1,0,0);

		int triangles[] = { 0, 1, 2, 0, 2, 3};

		for (int i = 0; i < 6; i++)
		{
			positions.push_back(vertexPositions[triangles[i]]);
			normals.push_back(faceNormal);
			tangents.push_back(faceTangent);
			uvs.push_back(vertexUVs[triangles[i]]);
		}
	}
public:
	QuadGeometry(float uvScale = 1.0f) { CreateGeometry(uvScale); }
	const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; }
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return positions.size(); }

};