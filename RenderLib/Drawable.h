#pragma once
#include "stdafx.h"
#include "Transforms.h"
#include "PointLight.h"

class Drawable
{
public:
	virtual void Draw(const Transforms&, const PointLight&) = 0;
};