#pragma once

#include<string>
#include<GL/glew.h>
#include "SpriteSheetInfo.h"
#include "AnimationParameters.h"

/* Animation.h
 * Encapsulates a single animation and it's intrinsic properties.
 */

class Animation{
public:
	Animation() = default;

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
	Animation(AnimationParameters& params, SpriteSheetParameters& spriteSheetParams, const std::string& d, const int facingDirection);
	Animation(GLuint image, int a, int b, int c, int c2, SpriteSheetParameters& spriteSheetParams, const std::string& d, const int facingDirection);
	int m_numberOfFrames; // number of frames in one row a the animation (all rows must contain the same number of frames)
	int m_numberOfRows;   // total number of rows for this animation
	int m_startingRow;    // the row this animation starts at on the spritesheet
	int m_startingCol;
	SpriteSheetParameters m_spriteSheetInfo;
	int m_currentFrame;   // initial value is 0
	int m_currentRow;     // initial value is 0
	float m_frameDivision;
	float m_rowDivision;
	float m_s1, m_s2, m_t1, m_t2;
	std::string m_name;
	int m_facingDirection;
	void nextFrame(float, float);   // draws the next frame of the animation
	GLuint m_image;
};
