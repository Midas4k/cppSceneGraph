#pragma once
#include "stdafx.h"
#include "GeometryHelper.h"

class SphereGeometry : public GeometryHelper
{
	// helper class which calculates vertex positions, normals and UV coordinates for a unit sphere
	// and gives them back in triangle order

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;

	void CreateGeometry(int tesselation)
	{
		// make 2D arrays of positions and UV coordinates
		glm::vec3 **vertexPos = new glm::vec3*[tesselation];
		glm::vec2 **vertexUV = new glm::vec2*[tesselation];
		glm::vec3** vertexTangent = new glm::vec3*[tesselation];

		for (int i = 0; i < tesselation; i++)
		{
			vertexPos[i] = new glm::vec3[tesselation];
			vertexUV[i] = new glm::vec2[tesselation];
			vertexTangent[i] = new glm::vec3[tesselation];
		}
		// calculate vertex positions and UV coordinates
		for (int i = 0; i < tesselation; i++)
		{
			float latitude = glm::radians(-90.0f) + glm::radians(180.0f) * (float)i / (tesselation - 1);
			for (int j = 0; j < tesselation; j++)
			{
				float longitude = glm::radians(360.0f) * (float)j / (tesselation - 1);
				float x, y, z;
				x = cos(latitude)*cos(longitude);
				y = sin(latitude);
				z = cos(latitude)*sin(longitude);
				vertexPos[i][j] = glm::vec3(x, y, z);
				vertexUV[i][j] = glm::vec2(1.0f - (float)j / tesselation, (float)i / tesselation);
				// calculate a tangent vector - make this from the cross product of the normal and up,
				// unless the normal *is* up/down
				if (y > -0.999f && y < 0.999f)
					vertexTangent[i][j] = glm::cross(vertexPos[i][j], glm::vec3(0, 1, 0));
				else
					vertexTangent[i][j] = glm::vec3(1, 0, 0);
			}
		}
		for (int i = 0; i < tesselation - 1; i++)
		{
			for (int j = 0; j < tesselation - 1; j++)
			{
				// the triangles look like this...
				/*

				(i,j+1)  (i+1,j+1)
				---------
				|      /|
				|  0  / |
				|    /  |
				|   /   |
				|  / 1  |
				| /     |
				---------
				(i,j)   (i+1,j)
				*/
				// triangle 0
				positions.push_back(vertexPos[i][j]); positions.push_back(vertexPos[i][j + 1]); positions.push_back(vertexPos[i + 1][j + 1]);
				uvs.push_back(vertexUV[i][j]); uvs.push_back(vertexUV[i][j + 1]); uvs.push_back(vertexUV[i + 1][j + 1]);
				tangents.push_back(vertexTangent[i][j]); tangents.push_back(vertexTangent[i][j + 1]); tangents.push_back(vertexTangent[i + 1][j + 1]);
				// triangle 1
				positions.push_back(vertexPos[i][j]); positions.push_back(vertexPos[i + 1][j + 1]); positions.push_back(vertexPos[i + 1][j]);
				uvs.push_back(vertexUV[i][j]); uvs.push_back(vertexUV[i + 1][j + 1]); uvs.push_back(vertexUV[i + 1][j]);
				tangents.push_back(vertexTangent[i][j]); tangents.push_back(vertexTangent[i + 1][j + 1]); tangents.push_back(vertexTangent[i + 1][j]);
			}
		}

		// clean up memory
		for (int i = 0; i < tesselation; i++)
		{
			delete vertexPos[i];
			delete vertexUV[i];
		}
		delete [] vertexPos;
		delete [] vertexUV;

	}
public:
	SphereGeometry(int tesselation) { CreateGeometry(tesselation); }
	const std::vector<glm::vec3>& GetPositionVector() const { return positions;  }
	const std::vector<glm::vec3>& GetNormalVector() const { return positions; }// same as positions 
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return positions.size();  }
};