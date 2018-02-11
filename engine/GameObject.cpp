#include "GameObject.h"



GameObject::GameObject()
{
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, float rotation) {
	this->pos = pos;
	this->size = size;
	this->velocity = velocity; 
	this->rotation = rotation;
}

GameObject::~GameObject()
{
}
void GameObject::update(float deltaTime) {
	pos += velocity*deltaTime;
	velocity += acceleration * deltaTime;
	if (pos.x < -size.x) {
		pos.x = 800;
	}
}

void GameObject::render() {
	sprite.draw(pos, size, rotation);
}

void GameObject::addHitBox(glm::vec2 pos, glm::vec2 size) {
	hitBoxes.push_back(new HitBox(pos, size));
}