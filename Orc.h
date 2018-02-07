#pragma once
#ifndef ORC_H
#define ORC_H

#include<GL/glew.h>
#include<stdio.h>
#include"DrawUtils.h"
#include"Sprite.h"
#include"Player.h"
#include"Observer.h"

class Orc : public Observer, public Sprite{
public:
	Orc();
	Orc(GLuint, float, float, int, int);
	void notify(Player*);
	void draw();
	void update(float);
private:
	GLuint image;
};

#endif