#pragma once

#include"Sprite.h"
#include"AnimationDef.h"
#include<GL/glew.h>
#include"DrawUtils.h"
#include"Observer.h"
#include<sstream>

/* AnimatedSprite.h
 * AnimatedSprite is a sprite that uses a list of animations to graphically represent it's
 * actions on screen. The AnimationDef member contains the list of animations that this 
 * AnimatedSprite uses and animation centric information common to all the animations.
 * AnimatedSprite can notify registered Observer objects. This allows other objects to
 * receive information about this object and interact accordingly.
 * */

class AnimatedSprite : public Sprite{
public:
	AnimatedSprite();
	AnimatedSprite(float, float, int, int, const std::string&);
	AnimatedSprite(float, float, int, int, const std::string&, class Game* game);
	AnimatedSprite(float, float, int, int, AnimationDef&, const std::string&);
	~AnimatedSprite(void);
	AnimationDef animationDef;
	virtual void update(float deltaTime);
	virtual void draw();
	void changeAnimation(int x); // tells animationDef to change to animation x
	int getCurrentAnimation() const;
	void notifyObservers();
	void registerObserver(Observer*);
	void unregisterObserver(Observer*);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stop();
	void setFacingDirection(int);
	int getFacingDirection() const;
	int facingDirection;
	enum facingDirections{LEFT, RIGHT, UP, DOWN};
	std::string name;
	std::string to_string() const;
	virtual void SetupAnimation() = 0;

protected:

	float speed_x;
	float speed_y;
	std::vector<Observer*> myObservers;
	std::map<std::string, int> m_animationMap;
	void SetAnimationDef(AnimationDef&);
	class Game* m_game;
};
