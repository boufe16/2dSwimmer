#pragma once
#include "sprite.h"
#include "GameObject.h"
#include <glm.hpp>
#include <math.h>
class player : public GameObject
{
public:
	player();
	~player();
	void update(float deltaT, float time, GLboolean *keys);
	void render(); 
	bool isInvuln = false;
	bool isDiving = false;
private:
	float opacity =1.0f;
};

