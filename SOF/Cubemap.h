#pragma once
#include "stdafx.h"
#include <vector>
#include <soil2.h>

namespace SOF
{
	class Cubemap
	{
		GLuint textureObject;
		void loadTextures(int num, const std::string& fileName);
	public:
		Cubemap(const std::vector<std::string> fileNames); // constructor, load from file
		GLuint GetObject() const { return textureObject; }
	};
}