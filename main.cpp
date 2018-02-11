#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream> 
#include <string>
#include "sprite.h"
#include "player.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "enemy.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include "UIElement.h"
#include "background.h"
#include "TextRenderer.h"

#define _W 800
#define _H 800
#define _Water 480
//***************************************************************************//
//Endless Swimmer 2D 
//Felix Bouchard
//2018

//window resized: 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//input processing :: 
void processInput(GLFWwindow * window, player *p);
//collision check with player and enemy 
bool collisions(GameObject &one, GameObject &two);
enum GameState { Menu, Game, GameOver };
//array of booleans tells us whether the key is pressed down or not
GLboolean Keys[1024];
int main() {
	//glfw will set the opengl version and greate a window 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the window obj 
	GLFWwindow * window = glfwCreateWindow(_W, _H, "Endless Swimmer 2D",
		NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//glad to manage function pointers for opengl 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//send opengl the size of the viewport 
	glViewport(0,0,_W,_H);
	glm::mat4 projection = glm::ortho(0.0f, (float)_W, (float)_H, 0.0f, -1.0f, 1.0f);
	
	
	
	TextRenderer textRenderer(_W, _H);
	//set callback 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	//load an image 

	//Sprite::SpriteSetup();
	Sprite::setProjectionMatrix(projection); 
	
	player p;
	
	//should instance all enemies 
	std::vector<enemy> enemyBank;
	enemy* e;
	for (int i = 0; i < 2; i++) {
		e = new enemy();
		e->size = glm::vec2(64, 64);
		e->pos = glm::vec2(_W, 480 - 50);
		e->velocity = glm::vec2(-100*i, 0);
		enemyBank.push_back(enemy(*e));
	}

	//wallpapers behind the sprites
	std::vector<background> rearBG;
	background * bg;
	//static
	bg = new background("X:/Assets/background_sky.png", "boringImageShader.vert", "boringImageShader.frag",0);
	bg->size = glm::vec2(_W, _H);
	rearBG.push_back(background(*bg));

	bg = new background("X:/Assets/water1.png", "boringImageShader.vert", "boringImageShader.frag", 50);
	bg->size = glm::vec2(_W, _H);
	rearBG.push_back(background(*bg));
	bg = new background("X:/Assets/water1.png", "boringImageShader.vert", "boringImageShader.frag", 50);
	bg->size = glm::vec2( _W, _H);
	bg->pos = glm::vec2(_W , 0);
	rearBG.push_back(background(*bg));

	bg = new background("X:/Assets/water2.png", "boringImageShader.vert", "boringImageShader.frag", 25);
	bg->size = glm::vec2(_W, _H);
	rearBG.push_back(background(*bg));
	bg = new background("X:/Assets/water2.png", "boringImageShader.vert", "boringImageShader.frag", 25);
	bg->size = glm::vec2(_W,_H);
	bg->pos = glm::vec2(_W , 0);
	rearBG.push_back(background(*bg));
	//wallpapers above the sprite 
	std::vector<background> frontBG;
	bg = new background("X:/Assets/water3.png", "boringImageShader.vert", "boringImageShader.frag", 75);
	bg->size = glm::vec2(_W, _H);
	frontBG.push_back(background(*bg));
	bg = new background("X:/Assets/water3.png", "boringImageShader.vert", "boringImageShader.frag", 75);
	bg->size = glm::vec2(_W, _H);
	bg->pos = glm::vec2(_W , 0);

	frontBG.push_back(background(*bg));


	//3 lives (just int for now) 
	int lives = 3;
	//game score 
	int score = 0;
	GameState gameState = Game; 
	UIElement hearts("X:/Assets/heartSprite.png", "boringImageShader.vert", "boringImageShader.frag");
	hearts.size = glm::vec2(32, 32);
	//buoy.myShader.setMat4("transform", buoyTrans);
	//buffer stuff 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	//turn off VSYNC
	glfwSwapInterval(1);

	//to have constant experiance (logic not based on framerate etc.. ); 
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float invulnStartTime = 0;
	//check if the window has been closed 
	while (!glfwWindowShouldClose(window)) {
		processInput(window, &p);
		//calculate new deltatime
		float currentFrame = glfwGetTime(); 
		deltaTime = currentFrame - lastFrame; 
		lastFrame = currentFrame; 
		//show fps
		//std::cout << "\r" << 1 / deltaTime << std::flush;
		//rendering commands 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//update player stuff
		p.update(deltaTime, currentFrame, Keys);
		//update the scenery 
		for (int i = 0; i < rearBG.size(); i++) {
			rearBG[i].update(deltaTime);
		}
		for (int i = 0; i < frontBG.size(); i++) {
			frontBG[i].update(deltaTime);
		}
		//iterate through the enemies and move them/ check for collisions
		for (int i = 0; i < enemyBank.size(); i++) {
			enemyBank[i].update(deltaTime);
			if (collisions(p, enemyBank[i])) {
				if (!p.isInvuln) {
					//std::cout << "\r HIT!" << std::flush;
					lives --;
					p.isInvuln = true;
					invulnStartTime = currentFrame;
				}
			}
			
			if (gameState != GameOver) {
			//if an enemy moves left past the player (completely)
				if ((enemyBank[i].pos.x + enemyBank[i].size.x)< p.pos.x && !enemyBank[i].pointMarked){
					//count point 
					score++; 
					//flag point as marked (so we don't double count) 
					enemyBank[i].pointMarked = true;

				}
			}
		}
		if (lives == 0) {
			//you lose
			gameState = GameOver;
		}

		if (p.isInvuln && ((currentFrame - invulnStartTime)>2) ) {
			p.isInvuln = false; 
		}

		//DRAW 
		//backgrounds 
		for (int i = 0; i < rearBG.size(); i++) {
			rearBG[i].render();
		}
		for (enemy e : enemyBank) {
			e.render();
		}

		p.render();

		//foreground
		for (int i = 0; i < frontBG.size(); i++) {
			frontBG[i].render();
		}
		//UI
		for (int i = 0; i < lives; i++) {
			hearts.pos = glm::vec2(i * (hearts.size.x + 5), 5);
			hearts.render();
		}
		//TEXT 
		//renders framerate as text
		std::stringstream stream;
		//float to string 
		stream << std::fixed  << std::setprecision(0) << (1.0/deltaTime);
		textRenderer.RenderText(stream.str(), 0, _H-64, 1, glm::vec3(0.0, 0.0, 0.0));

		//score display :: looks really shitty right now because of the sizing/antialiasing whatever 
		textRenderer.RenderText(std::to_string(score), (_W /2)-10, _H-64, 1.5, glm::vec3(1.0f, 1.0f, 1.0f));

		//
		if (gameState == GameOver) {
			textRenderer.RenderText("GAME OVER", 0, _H / 2 + 40, 2, glm::vec3(0, 0, 0));
		}
		glfwSwapBuffers(window);//swap colour buffer 
		glfwPollEvents(); //check if events triggered (keyboard/mouse etc) 
	}
	
	glfwTerminate();//delete allocated resources
	return 0; 
}
//on window resize 
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); 
}
bool collisions(GameObject &one, GameObject &two) {
		
	//objects can have many hitboxes. for now they are all equal(in damage etc)
	for (int i = 0; i < one.hitBoxes.size(); i++) {
		for (int j = 0; j < two.hitBoxes.size(); j++) {
			//make it a little easier on the eyes 
			glm::vec2 box1sz = one.hitBoxes[i]->sz;
			glm::vec2 box1Pos = one.hitBoxes[i]->relativePosition + one.pos;

			glm::vec2 box2sz = two.hitBoxes[j]->sz;
			glm::vec2 box2Pos = two.hitBoxes[j]->relativePosition + two.pos;

			//check collision and return true as soon as there is a collision.
			bool collisionX =
				box1Pos.x + box1sz.x >= box2Pos.x &&
				box2Pos.x + box2sz.x >= box1Pos.x;
			bool collisionY = box1Pos.y + box1sz.y >= box2Pos.y &&
				box2Pos.y + box2sz.y >= box1Pos.y;
			if (collisionX && collisionY)
				return true;
		}
	}
	return false;
}
//input processing here 


void processInput(GLFWwindow *window, player *p ) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		Keys[GLFW_KEY_SPACE] = true;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		Keys[GLFW_KEY_SPACE] = false; 
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
	//	glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
	//	
	//}
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
	//	glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
	//	//p->pos.x--;
	//}
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || 
	//	glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	//	if (p->pos.y > _Water-30 && !p->isDiving) {
	//		p->isDiving = true;
	//		p->acceleration.y = 0;//kill existing acceleration
	//		p->velocity.y = 100;
	//	}
	//}
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
	//	glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
	//	//p->pos.x++;
	//}
}