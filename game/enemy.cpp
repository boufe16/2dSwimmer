#include "enemy.h"



enemy::enemy():GameObject()
{
	this->sprite = Sprite("X:/Assets/buoy.png", "Shaders/boringImageShader.vert", "Shaders/boringImageShader.frag", false);
	//top part
	this->addHitBox(glm::vec2(24.0f, 0.0f), glm::vec2(14.0f, 48.0f));
	//liferaft part
	this->addHitBox(glm::vec2(9.0f, 47.0f), glm::vec2(44.0f, 17.0f));
}
enemy::~enemy()
{
}

void enemy::update(float deltaTime) {
	pos += velocity * deltaTime;
	velocity += acceleration * deltaTime;
	if (pos.x < -size.x) {
		pos.x = 800;
		//so we can mark it on next pass 
		pointMarked = false;
	}
}
