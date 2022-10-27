#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H
#include<string>
#include<GL/glew.h>

/* Animation.h
 * Encapsulates a single animation and it's intrinsic properties.
 */

class Animation{
public:
	Animation();

	/* Animation
	 * param image - the OpenGL image texture that contains the animation frames
	 * param a - numberOfFrames, the number of frames in one row of the animation. Multi row
	 *           animations must contain the same number of frames in each row
	 * param b - numberOfRows, the number of rows this animation consists of
	 * param c - startingRow, the row this animation starts at on the spritesheet
	 * param d - textual name of this animation
	 * param facingDirection - character facing direction shown in the animation
	 */
	Animation(GLuint image, int a, int b, int c, const std::string& d, const int facingDirection);
	int numberOfFrames; // number of frames in one row a the animation (all rows must contain the same number of frames)
	int numberOfRows;   // total number of rows for this animation
	int startingRow;    // the row this animation starts at on the spritesheet
	int currentFrame;   // initial value is 0
	int currentRow;     // initial value is 0
	float frameDivision;
	float rowDivision;
	float s1, s2, t1, t2;
	std::string name;
	int facingDirection;
	void nextFrame(float, float);   // draws the next frame of the animation
	GLuint image;
};
#endif