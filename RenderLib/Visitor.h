#pragma once
#include "stdafx.h"
#include "node.h"
#include "TransformNode.h"
#include "GeometryNode.h"
#include "LightNode.h"
#include "GroupNode.h"

class Visitor
{
protected:
	void VisitChildren(Node* n)
	{
		for (int i = 0; i < n->GetNumChildren(); i++)
			n->GetChild(i)->Accept(this);
	}
public:
	void Traverse(Node* root)
	{
		Reset();
		root->Accept(this);
	}
	virtual void Reset() {}
	virtual void Visit(TransformNode* n) { VisitChildren(n); }
	virtual void Visit(GeometryNode* n) { VisitChildren(n); }
	virtual void Visit(GroupNode* n) { VisitChildren(n); }
	virtual void Visit(LightNode* n) { VisitChildren(n); }
};
