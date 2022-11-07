#pragma once

#include<GL/glew.h>
#include<stdio.h>
#include"DrawUtils.h"
#include"Sprite.h"
#include"Observer.h"
#include"AnimatedSprite.h"

class StoreClerk : public Observer, public Sprite{
public:
	StoreClerk();
	StoreClerk(GLuint, float, float, int, int);
	void draw();
	void update(float);
	GLuint image;
	virtual void respondToObserved(AnimatedSprite*);
};
