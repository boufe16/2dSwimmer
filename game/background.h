#pragma once
#include "../engine/GameObject.h"
#include "../engine/sprite.h"
class background :public GameObject
{
public:
	background();
	background(std::string image, std::string vertexShaderPath, std::string personShaderPath, float scrollSpeed);
	~background();
};

