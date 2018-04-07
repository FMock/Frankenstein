#include"Skeleton.h"


Skeleton::Skeleton(float xPos, float yPos, int w, int h, AnimationDef& animDef, const std::string& n) :  Observer(),AnimatedSprite(xPos, yPos, w, h,animDef, n){
	moveLeft(); // initially move Skeleton left
}

void Skeleton::respondToObserved(AnimatedSprite* a){
	if(AABBIntersect(a->getBox()) && a->name == "player"){
		printf("Skeleton number %i Attacks\n", number);
	}
}

void Skeleton::update(float dt){
	if(x > 800){ //*****************************REMOVE 800, SETUP GAME WORLD CONSTANTS**************************************
		changeAnimation(animationDef.animationMap["walking_left"]);
		change_x = 0;
		moveLeft();
	}
	else if(x < 0){
		changeAnimation(animationDef.animationMap["walking_right"]);
		change_x = 0;
		moveRight();
	}

	x += change_x * dt;
	y += change_y * dt;
	position.setX(x);
	box.setX(abs(x));
	position.setY(y);
	box.setY(abs(y));
	animationDef.update(dt);
}