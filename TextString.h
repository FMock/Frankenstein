#pragma once
#ifndef TEXTSTRING_H
#define TEXTSTRING_H

#include<GL/glew.h>
#include"DrawUtils.h"
#include<string>
#include<cmath>

class TextString{
public:
	TextString();
	TextString(char* string, GLuint image, int imageWidth, int imageHeight, int frameWidth, int frameHeight, int x, int y);
	void drawText();

private:
	char* string;
	GLuint image;
	int width;  // image width
	int height; // image height
	int frameWidth;  // width of frame within image that gets drawn
	int frameHeight; // height of frame within image that gets drawn
	float s1, s2, t1, t2; // coords that specify area of image to draw
	int x, y, numColumns, numRows; //(x, y) = where to start drawing on screen
};

#endif