#pragma once
#include <glm.hpp>
#include <vector>
#include "sprite.h"
#include "hitBox.h"
#include <map>
#include "SpriteAnimation.h"

class GameObject
{
public:
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, float rotation);
	~GameObject();
	glm::vec2 pos = glm::vec2(1.0f);
	glm::vec2 size = glm::vec2(1.0f);
	glm::vec2 velocity = glm::vec2(1.0f);
	glm::vec2 acceleration = glm::vec2(0.0f);

	//this is a map that stores: string :animation, 
	//this way we can store n amount of animations 
	std::map <std::string, SpriteAnimation> AnimationMap;
	animationFrame *currentAnimationFrame;  
	std::vector<HitBox*> hitBoxes;
	float rotation = 0.0f;

	void addHitBox(glm::vec2 pos, glm::vec2 size);
	virtual void update(float deltaTime); 
	virtual void render();
protected:
	Sprite sprite; 
};

