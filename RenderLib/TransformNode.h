#pragma once
#include "stdafx.h"
#include "Node.h"
class TransformNode : public Node
{
public:
	glm::mat4 transform;
	TransformNode(const glm::mat4& transform) : transform(transform) {}
	const glm::mat4& GetTransform() const { return transform; }
	void SetTransform(const glm::mat4& t) { transform = t; }
	virtual void Accept(Visitor* v);
};