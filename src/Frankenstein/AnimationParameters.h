#pragma once
#include<GL/glew.h>
#include<string>
#include"SpriteSheetInfo.h"

struct AnimationParameters
{
	GLuint image; // texture
	int framesInAnimation;
	int rowsInAnimation;
	int startingRow;
	int startingCol;
	std::string animationName;
	int animationCount;
	//int facingDirection;
};