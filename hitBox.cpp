#include "hitBox.h"



HitBox::HitBox()
{
}

HitBox::HitBox(glm::vec2 pos, glm::vec2 size) {
	relativePosition = pos;
	sz = size;
}

HitBox::~HitBox()
{
}

