#include"Sprite.h"

Sprite::Sprite(void){}
Sprite::Sprite(float xPos, float yPos, int w, int h) : x(xPos), y(yPos), width(w), height(h){
	change_x = 0.0;
	change_y = 0.0;
}
Sprite::~Sprite(void){}

float Sprite::getX()const{
	return x;
}

float Sprite::getY()const{
	return y;
}

void Sprite::setX(float newX){
	x = newX;
}

void Sprite::setY(float newY){
	y = newY;
}