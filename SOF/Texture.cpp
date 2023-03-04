#include "stdafx.h"
#include "Texture.h"

SOF::Texture::Texture(int ntex)
{
	numText = ntex;
	textureObject = new GLuint[numText];
}

SOF::Texture::Texture(const std::string &fileName)
{
	textureObject = new GLuint[numText];
	loadTexture(0, fileName);
}

void SOF::Texture::loadTexture(int num, const std::string &fileName)
{

	width = 0;
	height = 0;
	unsigned char *image = SOIL_load_image(fileName.c_str(), &width, &height, NULL, SOIL_LOAD_RGBA);

	if (image == NULL)
	{
		std::cerr << "failed to load texture image from " << fileName << std::endl;
		return;
	}
	glGenTextures(num+1, &textureObject[num]);
	glBindTexture(GL_TEXTURE_2D, textureObject[num]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
}
