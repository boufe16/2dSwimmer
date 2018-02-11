#include "player.h"

//calls the Game object constructor with these values :: 
player::player() :GameObject(glm::vec2(100, 300), glm::vec2(64, 64), glm::vec2(0, 0), 0) {
	this->sprite = Sprite("X:/Assets/babySpriteSheet.png", "personShader.vert", "personShader.frag",false);
	//set the swimming animation 8 frames, 64 px jumps, 1 line 
	SpriteAnimation swim = SpriteAnimation(); 
	swim.animationDuration = 1; //1 sec duration
	swim.frameCount = 8; 
	for (int i = 0; i < swim.frameCount; i++) {
		animationFrame *af = new animationFrame{
			i*64.0f,//x
			0.0f,//y
			64.0f,//w
			64.0f,//h
			swim.animationDuration/swim.frameCount//duration
		};
		swim.animationFrames.push_back(*af);//add
	}
	AnimationMap["swim"] = swim;

	//I did these by drawing boxes in photoshop and writing down the values... 
	//I should come up with a way to do this using a better method... 
	//feet
	this->addHitBox(glm::vec2(1.0f, 10.0f), glm::vec2(9.0f, 25.0f));
	//body
	this->addHitBox(glm::vec2(9.0f, 19.0f), glm::vec2(51.0f, 19.0f));
	//head
	this->addHitBox(glm::vec2(49.0f, 8.0f), glm::vec2(14.0f, 15.0f));
}
float lastPosY = 0.0f;
int frameCount = 0;
float deltaTSum = 0;
void player::update(float deltaT, float time,GLboolean *keys) {
	
	//set current animation 
	currentAnimationFrame = &AnimationMap["swim"].animationFrames[frameCount];
	if (keys[GLFW_KEY_SPACE]) {
		if (pos.y > 450 && !isDiving) {
			isDiving = true;
			acceleration.y = 0;//kill existing acceleration
			velocity.y = 100;
		}
	}
	float ramp = 0.0f;
	deltaTSum += deltaT;
	if (isInvuln) {
		//when we are invulnerable our character flickers 
		// mario style
		opacity = sin(time*(50));
		if (opacity > 0)
			opacity = 1.0f;
		else
			opacity = 0.0f;
	}
	else {
		ramp = 0.0f;
		opacity = 1.0f;
	}
	if (pos.y > 480) {
		if (!isDiving &&lastPosY <= 480) {
			velocity.y = velocity.y*.5;
		}
		acceleration.y = -100;
	}
	else {
		if (!isDiving && lastPosY >= 480)
			velocity.y = velocity.y*.5;
		acceleration.y = 150;
	}
	//each frame lasts the amount of its duration 
	if (deltaTSum >currentAnimationFrame->duration) {
		//reset delta timer 
		deltaTSum = 0;
		//
		frameCount++; 
		//don't want to index further than we have to 
		if (frameCount == AnimationMap["swim"].frameCount)
			frameCount = 0; 
	}
	sprite.setTextureLocation(
		glm::vec2(currentAnimationFrame->x, currentAnimationFrame->y),
		glm::vec2(currentAnimationFrame->width, currentAnimationFrame->height),
		glm::vec2(512, 512)
	);
	if (isDiving) {
		//dive down
		if (lastPosY > pos.y) {//floating up
			acceleration.y = -200;
		}
		if (pos.y < 480) {
			isDiving = false; 
			if(keys[GLFW_KEY_SPACE])
				acceleration.y = -100;

		}
	}
	
	lastPosY = pos.y;
	pos += velocity * deltaT;
	velocity += acceleration * deltaT;
	if (pos.x < -size.x)
		pos.x = 800;
}

void player::render() {
	this->sprite.myShader.setFloat("opacityMod", this->opacity);
	this->sprite.draw(pos, size, rotation);
}

player::~player(){
}

