#pragma once
#include "shader.h"
#include "stb_image.h"
#include "SpriteAnimation.h"
class Sprite
{
public:
	void draw(glm::vec2 position, glm::vec2 size, GLfloat rotation);
	Sprite();
	Sprite(std::string texturePath, std::string vertexShaderPath, std::string fragmentShaderPath,bool flip = true);
	~Sprite();
	Shader myShader;
	int width, height, x, y;
	glm::mat4 transform;
	void setTextureLocation(glm::vec2 texturePos, glm::vec2 textureSize, glm::vec2 spriteSheetSize);
	void SpriteSetup();
	
	static void setProjectionMatrix(glm::mat4 proj);
private:
	unsigned int texture; 
	static glm::mat4 projection; 
	unsigned int VAO, TBO;
	//for our 2d game, the sprites will always be circle-based. 
	//making the vertex data static saves some space
	static unsigned int  VBO;
	static constexpr float vertices[] = {
		//pos		
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};
};
