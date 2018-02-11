#include "UIElement.h"


UIElement::UIElement() {

}
UIElement::UIElement(std::string image, std::string vertexShaderPath, std::string personShaderPath)
{
	this->sprite = Sprite(image, vertexShaderPath, personShaderPath, false);
}


UIElement::~UIElement()
{
}

void UIElement::update(float deltaTime) {
}
