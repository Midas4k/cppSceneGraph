#pragma once
#include "stdafx.h"
#include "GeometryHelper.h"

class CylinderGeometry : public GeometryHelper
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> uvs;

	void CreateGeometry(float height, int nDivisions)
	{
		glm::vec3* circle = new glm::vec3[nDivisions];
		glm::vec2* circleUV = new glm::vec2[nDivisions];
		for (int i = 0; i < nDivisions; i++)
		{
			float theta = glm::radians(360.0) * (float)i / (nDivisions - 1);
			circle[i] = glm::vec3(cos(theta), 0.0f, sin(theta));
			circleUV[i] = (glm::vec2(circle[i].x, circle[i].z) + glm::vec2(1.0f, 1.0f)) * 0.5f;
		}
		// top
		glm::vec3 normal = glm::vec3(0,1,0);
		glm::vec3 tangent = glm::vec3(1,0,0);

		for (int i = 0; i < nDivisions - 2; i++)
		{
			int i0, i1, i2; // indices for this face
			i0 = 0; i1 = i + 1; i2 = i + 2;

			positions.push_back(glm::vec3(circle[i0].x, height, circle[i0].z));
			positions.push_back(glm::vec3(circle[i1].x, height, circle[i1].z));
			positions.push_back(glm::vec3(circle[i2].x, height, circle[i2].z));
			uvs.push_back(circleUV[i0]); uvs.push_back(circleUV[i1]); uvs.push_back(circleUV[i2]);
			normals.push_back(normal); normals.push_back(normal); normals.push_back(normal);
			tangents.push_back(tangent); tangents.push_back(tangent); tangents.push_back(tangent);
		}

		// bottom
		normal = glm::vec3(0, -1, 0);
		tangent = glm::vec3(1, 0, 0);

		for (int i = 0; i < nDivisions - 2; i++)
		{
			int i0, i1, i2; // indices for this face
			i0 = 0; i1 = i + 1; i2 = i + 2;

			positions.push_back(glm::vec3(circle[i0].x, -height, circle[i0].z));
			positions.push_back(glm::vec3(circle[i1].x, -height, circle[i1].z));
			positions.push_back(glm::vec3(circle[i2].x, -height, circle[i2].z));
			uvs.push_back(circleUV[i0]); uvs.push_back(circleUV[i1]); uvs.push_back(circleUV[i2]);
			normals.push_back(normal); normals.push_back(normal); normals.push_back(normal);
			tangents.push_back(tangent); tangents.push_back(tangent); tangents.push_back(tangent);
		}
		// sides
		for (int i = 0; i < nDivisions; i++)
		{
			glm::vec3 pos[4]; glm::vec3 norm[4]; glm::vec3 tang[4]; glm::vec2 uv[4];
			// positions
			pos[0] = circle[i] + glm::vec3(0, -height, 0);
			pos[1] = circle[i] + glm::vec3(0, height, 0);
			pos[2] = circle[(i+1)%nDivisions] + glm::vec3(0, height, 0);
			pos[3] = circle[(i + 1) % nDivisions] + glm::vec3(0, -height, 0);
			// normals
			norm[0] = circle[i];
			norm[1] = circle[i];
			norm[2] = circle[(i+1)%nDivisions];
			norm[3] = circle[(i + 1) % nDivisions];
			// tangents
			tang[0] = glm::cross(norm[0], glm::vec3(0, 1, 0));
			tang[1] = glm::cross(norm[1], glm::vec3(0, 1, 0));
			tang[2] = glm::cross(norm[2], glm::vec3(0, 1, 0));
			tang[3] = glm::cross(norm[3], glm::vec3(0, 1, 0));
			// uv
			uv[0] = glm::vec2((float)i / nDivisions, 0);
			uv[1] = glm::vec2((float)i / nDivisions, 1);
			uv[2] = glm::vec2((float)(i + 1) / nDivisions, 1);
			uv[3] = glm::vec2((float)(i+1) / nDivisions, 0);

			int triangles[] = { 0, 1, 2, 0, 2, 3 };

			for (int i = 0; i < 6; i++)
			{
				positions.push_back(pos[triangles[i]]);
				normals.push_back(norm[triangles[i]]);
				tangents.push_back(tang[triangles[i]]);
				uvs.push_back(uv[triangles[i]]);
			}
		}
	}
public:
	CylinderGeometry(float height=1.0f, int nDivisions=16) { CreateGeometry(height, nDivisions); }
	const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; }
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return positions.size(); }

};