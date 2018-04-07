#include"Sprite.h"

Sprite::Sprite(void){}
Sprite::Sprite(float xPos, float yPos, int w, int h) : x(xPos), y(yPos), width(w), height(h), box(int(xPos), int(yPos), w, h),position(Vector2D(xPos, yPos)){
	change_x = 0.0;
	change_y = 0.0;
}
Sprite::~Sprite(void){}

float Sprite::getX()const{
	//return x;
	return position.getX();
}

float Sprite::getY()const{
	//return y;
	return position.getY();
}

void Sprite::setX(float newX){
	position.setX(newX);
	x = newX;
	box.setX(abs(x));
}

void Sprite::setY(float newY){
	position.setY(newY);
	y = newY;
	box.setY(abs(y));
}

AABB& Sprite::getBox(){
	return box;
}

bool Sprite::AABBIntersect(AABB& box2)const{
	// Check if this box is to the right of box2
	if(box.x > box2.x + box2.w){
		return false;
	}

	// Check if this box is to the left of box2
	if(box.x + box.w < box2.x){
		return false;
	}

	// Check if this box is below box2
	if(box.y > box2.y + box2.h){
		return false;
	}

	// Check if this box is above box2
	if(box.y + box.h < box2.y){
		return false;
	}
	
	return true;
}