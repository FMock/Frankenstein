#pragma once

#ifndef DRAWUTILS_H
#define DRAWUTILS_H

#include<string>


	GLuint glTexImageTGAFile(const char* filename);

	/* Uses default values for s and t texture coordinates since it assumes
	 * the full texture will be drawn*/
	void glDrawSprite(GLuint tex, int x, int y, int w, int h);

	/* This function is used when only part of a texture will be drawn. You must 
	 * pass the (s,t) texture coordinates */
	void glDrawFrame(GLuint tex, int x, int y, int w, int h, float s1, float s2, float t1, float t2);

	/*Draws each character image of string*/
	void drawRasterText(GLuint tex, int x, int y, int w, int h, char string[]);

	void glDrawSpriteRotate(GLuint tex, int x, int y, int w, int h, GLfloat angle);
	

#endif