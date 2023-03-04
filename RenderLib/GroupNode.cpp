#include "stdafx.h"
#include "GroupNode.h"
#include "Visitor.h"

// in implementation file to avoid circular depenency on header files
void GroupNode::Accept(Visitor* v)
{
	v->Visit(this);
}
