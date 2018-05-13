#pragma once

//#include "freetype/config/ftconfig.h"
//#include "freetype/config/ftoption.h"
//#include "freetype/freetype.h"
#include <ft2build.h>
#include FT_FREETYPE_H 
#include <iostream>

#include <map>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include "shader.hpp"

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
	void configureVertexData();
	void RenderText(Shader shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	void setProjection(int screenWidth, int screenHeight);
private:
	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;

	glm::mat4 projection;

};