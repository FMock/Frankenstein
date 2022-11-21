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
	//SpriteSheetInfo spriteSheetInfo;
	//std::string animationName;
	//int facingDirection;
};