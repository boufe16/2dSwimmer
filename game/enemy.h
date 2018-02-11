#pragma once
#include "sprite.h"
#include "GameObject.h"
class enemy : public GameObject
{

public:
	bool pointMarked = false ;
	enemy();
	~enemy();
	void update(float deltaTime);
};

