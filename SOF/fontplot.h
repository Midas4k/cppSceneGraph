#pragma once
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Font.h"

namespace SOF
{
	class FontPlot
	{
		ShaderProgram* shader;
		Font *pFont;
		Geometry *geometry;
		int screenWidth;
		int screenHeight;
		std::string vertexShaderSource =
		R"(#version 330

		uniform float xPos;
		uniform float yPos;
		uniform float xSize;
		uniform float ySize;

		uniform float uPos;
		uniform float vPos;
		uniform float uSize;
		uniform float vSize;

		in vec2 vert;
		in vec2 vertTexCoord;

		out vec2 fragTexCoord;

		void main() {
			// apply transform to vertex and uv
			fragTexCoord = vec2(vertTexCoord.x * uSize + uPos, vertTexCoord.y * vSize + vPos);
			gl_Position = vec4(vert.x * xSize + xPos, vert.y * ySize + yPos, 0.0f, 1.0f);
		}
		)";

		std::string fragmentShaderSource =
		R"(
		#version 330

		uniform sampler2D fontTex;
		uniform vec4 textColor;
		in vec2 fragTexCoord;

		out vec4 finalColor;

		void main() {
			finalColor = textColor * texture(fontTex, fragTexCoord);
		}
		)";

	public:
		typedef enum
		{
			SOF_JUSTIFY_LEFT,
			SOF_JUSTIFY_CENTRE,
			SOF_JUSTIFY_RIGHT
		} SOF_JUSTIFY;

		FontPlot(const std::string &font);
		void DrawChar(float x, float y, char c, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) )  const;
		void DrawString(float x, float y, const std::string &string, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), SOF_JUSTIFY just = SOF_JUSTIFY_LEFT) const;
	};
}