#include "stdafx.h"
#include "Cubemap.h"

SOF::Cubemap::Cubemap(const std::vector<std::string> fileNames)
{
	// expects six filenames. Expected order is ...
	GLint faces[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
					   GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
					   GL_TEXTURE_CUBE_MAP_POSITIVE_X,
					   GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
					   GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
					   GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };


	if (fileNames.size() != 6)
	{
		std::cerr << "SOF::Cubemap needs six file names" << std::endl;
		return;
	}
	glGenTextures(1, &textureObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureObject);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int width, height;
	for (int i = 0; i < 6; i++)
	{
		unsigned char* image = SOIL_load_image(fileNames[i].c_str(), &width, &height, NULL, SOIL_LOAD_RGBA);

		if (image == NULL)
		{
			std::cerr << "failed to load texture image from " << fileNames[i] << std::endl;
			return;
		}
		glTexImage2D(faces[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}
