#include "stdafx.h"
#include "Font.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

int SOF::Font::ParseLine(const std::string &line, SOF::Font::Glyph &glyph) const
{
	std::size_t found = line.find("char id=");
	if (found == std::string::npos)
	{
		return -1;
	}
	int id;
	sscanf_s(line.c_str() + found + 8, "%d", &id);
	found = line.find("x=");
	sscanf_s(line.c_str() + found + 2, "%d", &glyph.x);
	found = line.find("y=");
	sscanf_s(line.c_str() + found + 2, "%d", &glyph.y);
	found = line.find("width=");
	sscanf_s(line.c_str() + found + 6, "%d", &glyph.w);
	found = line.find("height=");
	sscanf_s(line.c_str() + found + 7, "%d", &glyph.h);
	found = line.find("xoffset=");
	sscanf_s(line.c_str() + found + 8, "%d", &glyph.xOffset);
	found = line.find("yoffset=");
	sscanf_s(line.c_str() + found + 8, "%d", &glyph.yOffset);
	found = line.find("xadvance=");
	sscanf_s(line.c_str() + found + 9, "%d", &glyph.xAdvance);
	return id;

}

int SOF::Font::ParseFile(const std::string &name) 
{
	std::ifstream inFile;
	inFile.open(name);

	if (inFile.is_open())
	{
		std::string line;
		SOF::Font::Glyph tempGlyph;
		int glyphIndex;
		while (std::getline(inFile, line))
		{
			if ((glyphIndex = ParseLine(line, tempGlyph)) != -1)
			{
				glyphs[glyphIndex] = tempGlyph;
			}
		}
		inFile.close();
		return 1;
	}
	return 0;

}
SOF::Font::Font(const std::string &name)
{
	// zero the glyph array
	memset(glyphs, 0, 256 * sizeof(Glyph));
	// we will be readinf name.fnt and name_0.png
	std::string fontFile = std::string(name) + std::string(".fnt");
	ParseFile(fontFile);
	// load the texture
	std::string textureFilename = std::string(name) + std::string("_0.png");
	atlas = new Texture(textureFilename);
	// turn off texture filtering
	glBindTexture(GL_TEXTURE_2D, atlas->GetObject() );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	texSize = atlas->GetWidth();
}