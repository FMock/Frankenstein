#pragma once

#ifndef DRAWUTILS_H
#define DRAWUTILS_H

#include<string>


	GLuint glTexImageTGAFile(const char* filename);

	void glDrawSprite(GLuint tex, int x, int y, int w, int h);

	void glDrawSpriteSheet(GLuint tex, int x, int y, int w, int h, float u, float v);

	void glDrawSpriteRotate(GLuint tex, int x, int y, int w, int h, GLfloat angle);
	

#endif