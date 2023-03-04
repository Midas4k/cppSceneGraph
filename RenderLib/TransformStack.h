#pragma once
#include "stdafx.h"

class TransformStack
{
	std::vector<glm::mat4> stack;
public:
	TransformStack() { stack.push_back(glm::mat4()); }
	void Clear() { stack.clear(); }
	void Push(const glm::mat4& t) { stack.push_back(GetModel() * t); }
	const glm::mat4& GetModel() const { return stack.back(); }
	void Pop() { stack.pop_back(); }
};