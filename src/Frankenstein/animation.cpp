#include"animation.h"

Animation::Animation(){}

/* Animation
* param image - The openGL texture that represents the frames of the animation
* param a - numberOfFrames, the number of frames in one row of the animation. Multi row
*           animations must contain the same number of frames in each row
* param b - numberOfRows, the number of rows this animation consists of
* param c - startingRow, the row this animation starts at on the spritesheet
* param d - name of this animation */
Animation::Animation(GLuint image, int a, int b, int c, const std::string& d, const int facingDir): image(image),
					numberOfFrames(a), numberOfRows(b), frameDivision(1.0/numberOfFrames),
					rowDivision(1.0/numberOfRows), startingRow(c), currentFrame(0),
					currentRow(startingRow), s1(0.0), s2(frameDivision), t1(currentRow*rowDivision), 
					t2((currentRow*rowDivision) + rowDivision), name(d), facingDirection(facingDir){}

void Animation::nextFrame(float frameTime, float animFPS){
	// Calculate how many frames to jump ahead
	currentFrame += frameTime * animFPS;

	// Keep currentFrame in range
	if(currentFrame >= numberOfFrames)
		currentFrame = currentFrame % numberOfFrames;

	// current row may change. Use modulus
	if(s2 >= 1.0 && numberOfRows > 1)
		currentRow = (currentRow + 1) % numberOfRows;

	// update s1, s2, t1, t2
	s1 = currentFrame * frameDivision;
	s2 = (currentFrame * frameDivision) + frameDivision;
	t1 = currentRow * rowDivision;
	t2 = (currentRow * rowDivision) + rowDivision;
}
