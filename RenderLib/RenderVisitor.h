#pragma once
#include "TransformStack.h"
#include "TransformNode.h"
#include "GeometryNode.h"
#include "LightNode.h"
#include "GroupNode.h"
#include "LightStack.h"
#include "Visitor.h"

class RenderVisitor : public Visitor
{
	TransformStack t;
	LightStack l;
	glm::mat4 view;
	glm::mat4 proj;
public:
	RenderVisitor(const glm::mat4& view, const glm::mat4& proj) : view(view), proj(proj) {}

	virtual void Visit(TransformNode* n)
	{
		t.Push(n->GetTransform());
		VisitChildren(n);
		t.Pop();
	}

	virtual void Visit(GeometryNode* n)
	{
		Transforms trans(t.GetModel(), view, proj);
//		trans.SetModel(t.GetModel());
//		trans.SetView(view);
//		trans.SetProj(proj);
		n->GetDrawable()->Draw(trans, l.GetLights());
		// assume geometry node is a leaf, so we don't call VisitChildren
	}

	virtual void Visit(LightNode* n)
	{
		l.Push(n, t.GetModel() );
		VisitChildren(n);
		l.Pop();
	}
};
