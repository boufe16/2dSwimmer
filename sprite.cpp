#include "sprite.h"


Sprite::Sprite() {
	SpriteSetup();
}
///sets a sprite with a new Texture 
Sprite::Sprite(
	std::string texturePath, 
	std::string vertexShaderPath,
	std::string fragmentShaderPath, 
	bool flip)
{
	SpriteSetup();
	//set the shader for the sprite (sprites have their own shaders)
	myShader = Shader(vertexShaderPath, fragmentShaderPath);
	int width, height, nrChannels; 
	stbi_set_flip_vertically_on_load(flip);
	unsigned char * data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0); 
	glGenTextures(1, &texture); 
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (data) {
		if (nrChannels == 3) {//rgb
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4) {//rgba
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "Only handle images with 3 or 4 channels" << std::endl;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Can't load Texture" << std::endl;
	}
	stbi_image_free(data);
	//set the texture stuff 
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
}

///Draw sprite with loaded Texture
void Sprite::draw(glm::vec2 position, glm::vec2 size, GLfloat rotation) {
	myShader.use();
	glm::mat4 model(1.0f);
	//move sprite to position
	model = glm::translate(model, glm::vec3(position,1.0f));

	//make the center of the sprite the "anchor" for rotation
	model = glm::translate(model, glm::vec3(0.5f*size.x, 0.5f*size.y, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));//rotate
	model = glm::translate(model, glm::vec3(-0.5f*size.x , -0.5f *size.y, 0.0f));//translate back

	model = glm::scale(model, glm::vec3(size, 1.0f));//make to desired size. 
	this->myShader.setMat4("model", model);
	this->myShader.setMat4("projection", this->projection);
	glBindVertexArray(this->VAO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

/// sets the textureLocation from within aspritesheet , 
/// Takes position and size as 2 vectors with integers representing 
/// the size and position of the target area in PIXELS (relative to corner)  
void Sprite::setTextureLocation(glm::vec2 texturePos, glm::vec2 textureSize, glm::vec2 spriteSheetSize) {
	// We need to normalize these Pixel values in floats from 0-1 
	// for that we need the size of the SpriteSheet(in pixels). 
	float textureX = texturePos.x/ spriteSheetSize.x;
	float textureY = texturePos.y/ spriteSheetSize.y;
	float textureW = textureSize.x/ spriteSheetSize.x;
	float textureH = textureSize.y/ spriteSheetSize.y;
	// define a new array of Texture Sample Location 
	// this will allow us to get a texture from a Spritesheet 
	// at a user-defined location
	float modifiedTextureLoc[] = {
		textureX,			textureY + textureH,
		textureX + textureW,  textureY,
		textureX,			textureY,

		textureX,			textureY + textureH,
		textureX + textureW,  textureY + textureH,
		textureX + textureW,  textureY
	};
	unsigned int VTTOO;
	glGenBuffers(1, &VTTOO);

	glBindVertexArray(VAO);
	//texture coord
	glBindBuffer(GL_ARRAY_BUFFER, VTTOO);
	//bind buffer with texture locations
	//Copy data into the bound buffer
	//this is just an initiation to the texVer Original value (rectangle to corners of the Texture) 
	glBufferData(GL_ARRAY_BUFFER, sizeof(modifiedTextureLoc), modifiedTextureLoc, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(VAO);
}

Sprite::~Sprite()
{
}

//unsigned int Sprite::VAO;
unsigned int Sprite::VBO;
//unsigned int Sprite::VTO;
glm::mat4 Sprite::projection;

void Sprite::setProjectionMatrix(glm::mat4 proj) {
	Sprite::projection = proj;
}
void Sprite::SpriteSetup() {

	float texVert[12] = {//always a square 
						 //tex
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &TBO);
	glGenVertexArrays(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//bind buffer. 
	//Copy data into the bound buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//linking the vertex attributes
	//position
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	
	//texture coord
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	//bind buffer with texture locations
	//Copy data into the bound buffer
	//this is just an initiation to the texVer Original value (rectangle to corners of the Texture) 
	glBufferData(GL_ARRAY_BUFFER, sizeof(texVert), texVert, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);//assign buffer to Location 1 (found in vert shader) 

	//if i end up using different vertices, I will need to swap the buffers in the DRAW functions
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(VAO);
}