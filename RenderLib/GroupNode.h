#pragma once
#include "stdafx.h"
#include "Node.h"

class GroupNode : public Node
{
public:
	virtual void Accept(Visitor* v);
};