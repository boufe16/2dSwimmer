#pragma once
#include "../engine/sprite.h"
#include "../engine/GameObject.h"
class enemy : public GameObject
{

public:
	bool pointMarked = false ;
	enemy();
	~enemy();
	void update(float deltaTime);
};

