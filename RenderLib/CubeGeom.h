#pragma once
#include "stdafx.h"
#include "GeometryHelper.h"

class CubeGeometry : public GeometryHelper
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;

	void CreateGeometry()
	{
		glm::vec3 vertexPositions[] = {
			glm::vec3(-1, -1, -1),
			glm::vec3(-1, -1,  1),
			glm::vec3( 1, -1,  1),
			glm::vec3( 1, -1, -1),
			glm::vec3(-1,  1, -1),
			glm::vec3(-1,  1,  1),
			glm::vec3( 1,  1,  1),
			glm::vec3( 1,  1, -1)
		};
		
		glm::vec3 faceTangents[] =
		{
			glm::vec3(0,0,1), glm::vec3(1,0,0), glm::vec3(0,0,-1),
			glm::vec3(-1,0,0), glm::vec3(1,0,1), glm::vec3(-1,0,0)
		};

		glm::vec3 faceNormals[] =
		{
			glm::vec3(-1,0,0), glm::vec3(0,1,0), glm::vec3(1,0,0),
			glm::vec3(0,0,-1), glm::vec3(0,1,0), glm::vec3(0,-1,0)
		};

		glm::vec2 faceUVs[] =
		{
			glm::vec2(0,0),glm::vec2(0,1),glm::vec2(1,1),glm::vec2(0,0),glm::vec2(1,1),glm::vec2(1,0) 
		};

		int triangles[] = { 0,4,5,0,5,1, 1,5,6,1,6,2, 2,6,7,2,7,3, 3,7,4,3,4,0, 4,7,6,4,6,5, 3,0,1,3,1,2 };

		for (int i = 0; i < 36; i++)
		{
			positions.push_back(vertexPositions[triangles[i]]);
			normals.push_back(faceNormals[i / 6]);
			tangents.push_back(faceTangents[i / 6]);
			uvs.push_back(faceUVs[i % 6]);
		}
	}
public:
	CubeGeometry() { CreateGeometry(); }
	const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; } 
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return positions.size(); }

};