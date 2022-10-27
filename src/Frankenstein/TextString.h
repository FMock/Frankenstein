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
	void Initialize(const char* string, GLuint image, int imageWidth, int imageHeight, int frameWidth, int frameHeight, int x, int y);
	void drawText();

private:
	const char* m_string;
	GLuint m_image;
	int m_width;  // image width
	int m_height; // image height
	int m_frameWidth;  // width of frame within image that gets drawn
	int m_frameHeight; // height of frame within image that gets drawn
	float m_s1, m_s2, m_t1, m_t2; // coords that specify area of image to draw
	int m_x, m_y, m_numColumns, m_numRows; //(x, y) = where to start drawing on screen
};

#endif