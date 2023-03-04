#pragma once
#include "Node.h"
#include "Drawable.h"

class GeometryNode : public Node
{
public:
	Drawable* drawable;
	GeometryNode(Drawable* drawable) : drawable(drawable) {}
	Drawable* GetDrawable() const { return drawable; }
	virtual void Accept(Visitor* v);
};