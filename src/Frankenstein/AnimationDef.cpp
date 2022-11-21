#include"AnimationDef.h"
#include"animation.h"

AnimationDef::AnimationDef(){}

AnimationDef::AnimationDef(int b, int c, int d, std::vector<Animation>& anims, const std::map<std::string, int>& aMap) : 
						   totalRowsOnSpriteSheet(b), frameWidth(c), frameHeight(d),
						   currentAnimation(0), previousAnimation(0), frameTime(0.0f), 
						   animFPS(10.0f), animations(anims), numberOfAnimations(animations.size()), animationMap(aMap) {}

void AnimationDef::update(float deltaTime){
	// update how long the current frame has been displayed
	frameTime += deltaTime;

	// time to switch to the next frame?
	if (frameTime > (1/animFPS)){
		animations.at(currentAnimation).nextFrame(frameTime, animFPS);
		frameTime = fmod(frameTime, 1 / animFPS);
	}
}

int AnimationDef::getCurrentAnimation()const{
	return currentAnimation;
}

void AnimationDef::setCurrentAnimation(int a){
	if(a >= 0 && a < animations.size())
		currentAnimation = a;
}

int AnimationDef::getFrameWidth()const{
	return frameWidth;
}

int AnimationDef::getFrameHeight()const{
	return frameHeight;
}

// returns the facing direction of the current animation
int AnimationDef::getFacingDirection()const{
	return animations.at(currentAnimation).m_facingDirection;
}