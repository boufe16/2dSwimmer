#include "background.h"

background::background() {

}


background::background(std::string image, std::string vertexShaderPath, std::string personShaderPath, float scrollSpeed)
{
	this->sprite = Sprite(image, vertexShaderPath, personShaderPath, false);
	velocity = glm::vec2(-scrollSpeed, 0.0f);
}


background::~background()
{
}

