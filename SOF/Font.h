#pragma once
#include "Texture.h"
namespace SOF
{
	class Font
	{
		// bitmap font - loads font description + texture atlas produced by AngelCode bitmap font tool
		struct Glyph
		{
			int x, y, w, h, xOffset, yOffset, xAdvance;
		};
		Glyph glyphs[256];  // all the glyphs, up to 256
		Texture *atlas;
		int texSize;
		int ParseLine(const std::string &line, Glyph &glyph) const;
		int ParseFile(const std::string &fileName);
	public:
		Font(const std::string &name );
		int GetX(char c) const { return glyphs[c].x; }
		int GetY(char c) const { return glyphs[c].y; }
		int GetW(char c) const { return glyphs[c].w; }
		int GetH(char c) const { return glyphs[c].h; }
		int GetXOffs(char c) const { return glyphs[c].xOffset; }
		int GetYOffs(char c) const { return glyphs[c].yOffset; }
		int GetXAdvance(char c) const { return glyphs[c].xAdvance; }
		int GetTexSize() const { return texSize; }
		const Texture *GetTexture() const { return atlas; }
	};
}