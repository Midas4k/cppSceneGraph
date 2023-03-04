#pragma once
#include "stdafx.h"
#include "GeometryHelper.h"

class satRing : public GeometryHelper {
	//This is the creation of the Vectors that we will use
	std::vector<glm::vec3> positions; //posistions will be where we store the points for the objects
	std::vector<glm::vec3> normals; //the normals show which way the object is facing
	std::vector<glm::vec3> tangents;//this shows the tangent for the object
	std::vector<glm::vec2> uvs;//Lastly this will allow us to display a texture on the object

	void CreateGeometry(float height , float width  , int nDivisions){
		glm::vec3* farEdge = new glm::vec3[nDivisions];//This is the outer circles edge/ vertex positions
	    glm::vec3* closeEdge = new glm::vec3[nDivisions];//additionally we need an inner circle edge/ vertex positions
		glm::vec2* farUVs = new glm::vec2[nDivisions];//as above so below, however this is with the UV's instead
		glm::vec2* closeUVs = new glm::vec2[nDivisions];

		for (int i = 0; i < nDivisions; i++) {
			float theta = glm::radians(360.0f) * (float)i / (nDivisions - 1);

			farEdge[i] = glm::vec3(cos(theta), .0f, sin(theta)); //here we are setting the points on the outer edge that will be used for the shape
			closeEdge[i] = farEdge[i] * (1.0f - width);// to make the inner circle we simply take the outer edge and reduce it by a passed through width

		}

		//top
		glm::vec3 normal = glm::vec3(0, 1, 0);
		glm::vec3 tangent = glm::vec3(1, 0, 0);


		//We then create a loop to store the information for later, this is very much the same as the CylinderGeom since thats the style we are aiming for
		for (int i = 0; i < nDivisions - 1; i++) {

			positions.push_back(glm::vec3(closeEdge[i].x, height, closeEdge[i].z)); positions.push_back(glm::vec3(farEdge[i].x, height, farEdge[i].z)); 
			positions.push_back(glm::vec3(farEdge[i + 1].x, height, farEdge[i + 1].z));
			positions.push_back(glm::vec3(closeEdge[i].x, height, closeEdge[i].z)); positions.push_back(glm::vec3(farEdge[i + 1].x, height, farEdge[i + 1].z)); 
			positions.push_back(glm::vec3(closeEdge[i + 1].x, height, closeEdge[i + 1].z));
			uvs.push_back(glm::vec2(0,0)); uvs.push_back(glm::vec2(1, 0)); uvs.push_back(glm::vec2(1, 1));
			uvs.push_back(glm::vec2(0, 0)); uvs.push_back(glm::vec2(1, 1)); uvs.push_back(glm::vec2(0,1));
			normals.push_back(normal); normals.push_back(normal); normals.push_back(normal);
			normals.push_back(normal); normals.push_back(normal); normals.push_back(normal);
			tangents.push_back(tangent); tangents.push_back(tangent); tangents.push_back(tangent);
			tangents.push_back(tangent); tangents.push_back(tangent); tangents.push_back(tangent);
		}

	}
public:
	satRing(float height = .1f, float width = .3f, int nDivisions = 16) { CreateGeometry(height, width, nDivisions); }
	const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; }
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return positions.size(); }
};