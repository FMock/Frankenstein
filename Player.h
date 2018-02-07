#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include<GL/glew.h>
#include<vector>
#include"DrawUtils.h"
#include"Observer.h"
#include"Sprite.h"
#include"Vector2D.h"
#include<string>
#include<sstream>

class Player : public Sprite{
public:
	Player();
	Player(GLuint, float, float, int, int);
	void draw();
	void update(float);
	void notifyObservers();
	void registerObserver(Observer*);
	void unregisterObserver(Observer*);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stop();
	inline bool getRotate()const{return rotate;}
	inline void setRotate(bool r){rotate = r;}
	std::string to_string() const;

private:
	GLuint image;
	float speed_x;
	float speed_y;
	std::vector<Observer*> myObservers;
	bool rotate;
	GLfloat rotateAngle;
	Vector2D position;
};

#endif