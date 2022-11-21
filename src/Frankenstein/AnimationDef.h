#ifndef ANIMATIONDEF_H
#define ANIMATIONDEF_H

#include"animation.h"
#include<vector>
#include<map>
#include<string>

class AnimationDef{
public:
	AnimationDef();

	/* AnimationDef.h
	 * param b - total rows on the spritesheet
	 * param c - frameWidth
	 * param d - frameHeight
	 * param anims - a reference to a vector of animation objects
	 * param aMap - mapping of animation names to it's index in the aniamtions vector
	 * The animationMap member contains a mapping of each animations textual name to it's
	 * index position in the animation list. main.cpp can use this mapping to do a lookup to check
	 * and possibly change an animation when the player presses certain keys.
	 */
	AnimationDef(int b, int c, int d, std::vector<Animation>& anims, const std::map<std::string, int>& aMap);
	//int currentAnimation;   // The current animation playing. initially set to 0
	int previousAnimation;
	float frameTime; // the amount of time the current frame has been displayed
	float animFPS;   // default is 24.0f
	int getFrameWidth()const;
	int getFrameHeight()const;
	int getCurrentAnimation()const;
	void setCurrentAnimation(int);
	int getFacingDirection()const;
	std::vector<Animation> animations;
	void update(float deltaTime);
	std::map<std::string, int> animationMap;
private:
	int numberOfAnimations; // The number of animations this object manages
	int totalRowsOnSpriteSheet; // The total number of rows on the sprite sheet
	int currentAnimation;   // The current animation playing. initially set to 0
	int frameWidth;  // all frames on the sprite sheet should have the same width
	int frameHeight; // all frames on the sprite sheet should have the same height
};

#endif