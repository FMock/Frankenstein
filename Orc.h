#pragma once
#ifndef ORC_H
#define ORC_H

#include<GL/glew.h>
#include<stdio.h>
#include"DrawUtils.h"
#include"Sprite.h"
#include"Observer.h"
#include"AnimatedSprite.h"

class Orc : public Observer, public Sprite{
public:
	Orc();
	Orc(GLuint, float, float, int, int);
	int number;
	void draw();
	void update(float);
	GLuint image;
	virtual void respondToObserved(AnimatedSprite*);
protected:
	inline void observerResponse(){ printf("Attack!\n"); }
};

#endif