#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm.hpp>
#include <map>
#include <iostream>
#include "shader.h"
#include "stb_image.h"
class TextRenderer
{
public:

	
	TextRenderer(int windowWidth, int windowHeight);
	~TextRenderer();
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
private : 
	//a character struct to store the text values (only first 128 characters)
	struct Character {
		GLint TextureID;//id of the glyph's texture 
		glm::ivec2 Size; //size of glyph
		glm::ivec2 Bearing; //offset from baseline (0,0) some letters go below 0 (g,p etc) 
		GLint Advance; //Offset to advance to next glyph
	};
	std::map <char, Character> Characters;
	//Text is not exactly like sprite so we define a new set of VAO and VBO 
	unsigned int VAO, VBO;
	Shader textShader;
	float sWidth, sHeight;
};


