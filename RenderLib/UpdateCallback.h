#pragma once
#include "stdafx.h"

// abstract base class for update callbacks
class UpdateCallback
{
public:
	virtual void operator()(float dt) = 0;
};