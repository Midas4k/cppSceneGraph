#pragma once

#pragma once
#include "TransformStack.h"
#include "TransformNode.h"
#include "GeometryNode.h"
#include "LightNode.h"
#include "GroupNode.h"
#include "LightStack.h"
#include "Visitor.h"
#include "UpdateCallback.h"

class UpdateVisitor : public Visitor
{
	float dt;

	void GenericVisit(Node* n)
	{
		if (n->GetUpdateCallback() != nullptr)
			(*n->GetUpdateCallback())(dt);
		VisitChildren(n);
	}
public:
	UpdateVisitor(float dt) : dt(dt) {}
	virtual void Visit(TransformNode* n)
	{
		GenericVisit(n);
	}
	virtual void Visit(GeometryNode* n)
	{
		GenericVisit(n);
	}

	virtual void Visit(LightNode* n)
	{
		GenericVisit(n);
	}

	virtual void Visit(GroupNode* n)
	{
		GenericVisit(n);
	}
};
