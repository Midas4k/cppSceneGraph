#include "stdafx.h"
#include "TransformNode.h"
#include "Visitor.h"

// in implementation file to avoid circular depenency on header files
void TransformNode::Accept( Visitor *v )
{ 
	v->Visit(this); 
}
