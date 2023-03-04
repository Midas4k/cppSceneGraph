#include "stdafx.h"
#include "GeometryNode.h"
#include "Visitor.h"

// in implementation file to avoid circular depenency on header files
void GeometryNode::Accept(Visitor* v)
{
	v->Visit(this);
}
