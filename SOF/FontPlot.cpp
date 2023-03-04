#include "stdafx.h"
#include "fontplot.h"
#include "shaderprogram.h"
#include "window.h"

SOF::FontPlot::FontPlot(const std::string& font)
{
	screenWidth = SOF::Window::GetWidth();
	screenHeight = SOF::Window::GetHeight();
	// load the shaders
	shader = new SOF::ShaderProgram(vertexShaderSource, fragmentShaderSource);
	// load the font
	pFont = new SOF::Font(font);
	// initialize the vertex data - this will be a quad, 0 to 1 in both directions, and uv.
	// these positions and uvs are transformed by shader uniforms on plotting
	// note - v's are flipped
	glm::vec2 pos[6];
	glm::vec2 uv[6];

	pos[0] = glm::vec2(0.0f, 0.0f);	pos[1] = glm::vec2(0.0f, 1.0f);	pos[2] = glm::vec2(1.0f, 1.0f);
	pos[3] = glm::vec2(0.0f, 0.0f); pos[4] = glm::vec2(1.0f, 1.0f);	pos[5] = glm::vec2(1.0f, 0.0f);
	uv[0] = glm::vec2(0.0f, 1.0f); uv[1] = glm::vec2(0.0f, 0.0f); uv[2] = glm::vec2(1.0f, 0.0f);
	uv[3] = glm::vec2(0.0f, 1.0f); uv[4] = glm::vec2(1.0f, 0.0f); uv[5] = glm::vec2(1.0f, 1.0f);
	// load shader
	shader = new ShaderProgram(vertexShaderSource, fragmentShaderSource);
	geometry = new Geometry(6);
	geometry->AddAttribute(std::vector<glm::vec2>(pos, pos+6), "vert");
	geometry->AddAttribute(std::vector<glm::vec2>(uv, uv+6), "vertTexCoord");
	geometry->Finalize(shader);
}

void SOF::FontPlot::DrawChar(float x, float y, char c, glm::vec4 color) const
{
	//bind the shaders
	shader->Use();

	shader->AssignTextureSampler("fontTex", pFont->GetTexture());

	// work out the screen coordinates here
	float w, h, xo, yo;
	// note: screen goes -1 to 1 in both directions (normalized device coordinates). x, y passed in in pixels, width height etc from font also in pixels.
	// both need converting using the screen width and height.
	x = 2.0f*(x - screenWidth * 0.5f) / screenWidth;
	y = 2.0f*(y - screenHeight * 0.5f) / screenHeight;
	w = 2.0f*(float)pFont->GetW(c) / screenWidth;
	h = 2.0f*(float)pFont->GetH(c) / screenHeight;
	xo = 2.0f*(float)pFont->GetXOffs(c) / screenWidth;
	yo = 2.0f*(float)pFont->GetYOffs(c) / screenHeight;

	float u0, v0, uSize, vSize;
	u0 = (float)pFont->GetX(c) / pFont->GetTexSize();
	v0 = (float)pFont->GetY(c) / pFont->GetTexSize();
	uSize = (float)pFont->GetW(c) / pFont->GetTexSize();
	vSize = (float)pFont->GetH(c) / pFont->GetTexSize();
	// set shader uniforms for transform
	shader->SetUniformFloat("xPos", x + xo);
	shader->SetUniformFloat("yPos", y - h - yo);
	shader->SetUniformFloat("xSize", w);
	shader->SetUniformFloat("ySize", h);
	shader->SetUniformFloat("uPos", u0);
	shader->SetUniformFloat("vPos", v0);
	shader->SetUniformFloat("uSize", uSize);
	shader->SetUniformFloat("vSize", vSize);
	shader->SetUniformVec4("textColor", color);
	// turn off z test
	glDepthFunc(GL_ALWAYS);
	// draw the geometry
	geometry->DrawPrimitives();
	// reenable z buffer
	glDepthFunc(GL_LESS);
}

void SOF::FontPlot::DrawString(float x, float y, const std::string &text, glm::vec4 color, SOF::FontPlot::SOF_JUSTIFY just) const
{
	int stringWidth = 0;
	if (just != SOF_JUSTIFY_LEFT)
	{
		// calculate width of string in pixels and offset x
		for (int i = 0; i < (int) text.length(); i++)
		{
			stringWidth += pFont->GetXAdvance(text[i]);
		}
		if (just == SOF_JUSTIFY_CENTRE)
			x -= stringWidth / 2;
		else if (just == SOF_JUSTIFY_RIGHT)
			x -= stringWidth;
	}
	for (int i = 0; i < (int) text.length(); i++)
	{
		DrawChar(x, y, text[i], color);
		x += pFont->GetXAdvance(text[i]);
	}
}



