#pragma once

//#include "freetype/config/ftconfig.h"
//#include "freetype/config/ftoption.h"
//#include "freetype/freetype.h"
#include <ft2build.h>
#include FT_FREETYPE_H 
#include <iostream>

#include <map>
#include "glm.hpp"
#include "GL/glew.h"

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};

class TextController {
public:
	TextController();
	void init();

private:
	std::map<GLchar, Character> Characters;

};