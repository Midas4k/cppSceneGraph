#pragma once
#include "stdafx.h"
#include "app.h"
#include "RenderLib.h"
#include "Physics.h"

class Week5App : public SOF::App
{
	// geometry
	TexturedLit* floorQuad;
	TexturedLit* cuboid;

	SOF::ShaderProgram* shader;
	// camera
	glm::vec3 eye;    
	glm::vec3 lookAt; 
	glm::vec3 up;     
	FlyCam* camera;

	// scene graph
	Node* sceneGraphRoot;

	//collide
	Physics* physics;
	GroupNode* dynamicObjectNode;
	TransformNode* projectileNode;

public:
	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaT);
	void Shoot();
};