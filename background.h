#pragma once
#include "GameObject.h"
#include "sprite.h"
class background :public GameObject
{
public:
	background();
	background(std::string image, std::string vertexShaderPath, std::string personShaderPath, float scrollSpeed);
	~background();
};

