#pragma once
#include <glm.hpp>

class HitBox
{
public:
	HitBox();
	HitBox(glm::vec2 pos, glm::vec2 size);
	~HitBox();
	//position of the HitBox RELATIVE TO GAMEOBJECT
	glm::vec2 relativePosition; 
	//size of hitbox
	glm::vec2 sz;
};

