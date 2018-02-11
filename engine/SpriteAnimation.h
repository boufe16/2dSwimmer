#pragma once
#include <vector>
struct animationFrame {
	//location/size of frame on the spritesheet 
	float x;
	float y; 
	float width; 
	float height; 
	//timing information 
	float duration; 
};
class SpriteAnimation
{
public:
	std::vector<animationFrame> animationFrames; 
	float animationDuration;
	int frameCount;
	SpriteAnimation();
	~SpriteAnimation();
};
//holds the information about a single animation frame
