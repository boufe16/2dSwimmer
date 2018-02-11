#pragma once
#include "GameObject.h"
#include "sprite.h"
class UIElement: public GameObject
{
public:
	UIElement();
	UIElement(std::string image, std::string vertexShaderPath, std::string personShaderPath);
	~UIElement();
	void update(float deltaTime);
};

