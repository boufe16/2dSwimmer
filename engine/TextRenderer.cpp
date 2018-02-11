#include "TextRenderer.h"



TextRenderer::TextRenderer(int windowWidth, int windowHeight)
{
	//set window size of class 
	sWidth = windowWidth; 
	sHeight = windowHeight;
	//Font stuff goes here 
	//load lib 
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERR:Freetype: Could not init FT library" << std::endl;
	//load font face
	FT_Face face;
	if (FT_New_Face(ft, "C:/Users/Felix/Documents/open-sans/OpenSans-Regular.ttf", 0, &face))
		std::cout << "ERR:FreeType: Could not load font" << std::endl;

	//set the size in pixels, if we set the width to 0, it will automatically 
	//calculate it based on the height 
	FT_Set_Pixel_Sizes(face, 0, 48);

	//Set the active Character ; 
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		std::cout << "ERR:FreeType Could not load Character " << std::endl;
	//FT_LOAD_RENDER 
	//since we only used GL_RED above, we need to do : 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++) {
		//Set the active Character ; 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			std::cout << "ERR:FreeType Could not load Character " << std::endl;

		unsigned int Texture; 
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			//internal format red 
			//bitmap created is greyscale
			//only need a single byte
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//store for use later 
		Character character = {
			Texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}

	
	//garbage collection 
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	//set the shader
	textShader = Shader("Shaders/textShader.vert", "Shaders/textShader.frag");
	// set a projection matrix to be the size of the window
	//the shader will not make use of any perspective, orthogonal for text
	glm::mat4 projection = glm::ortho(0.0f, sWidth, 0.0f, sHeight);
	textShader.setMat4("projection", projection); 
	
	//set the buffers (VBO, VAO) 
	glGenVertexArrays(1, &VAO); 
	glGenBuffers(1, &VBO); 
	glBindVertexArray(VAO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//2d Quad (image texture for characters) require 6 vertices of 4 floats
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW); 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
	//set my render state, incl. color
	textShader.setVec3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	//loop through my string
	std::string::const_iterator c; 
	for (c = text.begin(); c != text.end(); c++) {
		//get the character infomation
		Character ch = Characters[*c];

		//Bearing = distance from origin point
		GLfloat xpos = x + ch.Bearing.x *scale;
		//letter might have component below origin
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		//update VBO for each Character 
		GLfloat vertices[6][4] = {
			//first triangle
			{xpos,	   ypos + h,	0.0, 0.0 },
			{xpos,	   ypos,		0.0, 1.0},
			{xpos + w, ypos,		1.0, 1.0},
			//second
			{xpos,	   ypos + h,	0.0,0.0},
			{xpos + w, ypos,		1.0,1.0},
			{xpos + w, ypos + h,	1.0, 0.0}
		};
		//set character's tex
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		//update the VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
		glBindBuffer(GL_ARRAY_BUFFER, 0); 
		//draw them to screen: 
		glDrawArrays(GL_TRIANGLES, 0, 6); 

		//need to move the X starting position to the right for 1 glyph:
		//advance is 1/64 px , we bitshift by 6 to get 1px 64 (2^6 = 64)
		x += (ch.Advance >> 6) *scale; 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

TextRenderer::~TextRenderer()
{
}
