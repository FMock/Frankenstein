#include"Skeleton.h"
#include "Game.h"

Skeleton::Skeleton(float xPos, float yPos, int w, int h, const std::string& n) : Observer(), AnimatedSprite(xPos, yPos, w, h, n)
{
	SetupAnimation();
	moveLeft(); // initially move Skeleton left
}

Skeleton::Skeleton(float xPos, float yPos, int w, int h, const std::string& n, Game* game) : Observer(), AnimatedSprite(xPos, yPos, w, h, n, game)
{
	SetupAnimation();
	moveLeft(); // initially move Skeleton left
}

Skeleton::Skeleton(float xPos, float yPos, int w, int h, AnimationDef& animDef, const std::string& n) :  Observer(),AnimatedSprite(xPos, yPos, w, h,animDef, n){
	SetupAnimation();
	moveLeft(); // initially move Skeleton left
}

void Skeleton::respondToObserved(AnimatedSprite* a){
	if(AABBIntersect(a->getBox()) && a->name == "player"){
		printf("Skeleton number %i Attacks\n", number);
	}
}

void Skeleton::SetupAnimation()
{
	m_animationMapSkeleton["walking_left"] = 0;
	m_skeletonWalkLeft = Animation(glTexImageTGAFile("../../images/skeleton_walking_left.tga"), 3, 1, 0, "walking_left", m_animationMapSkeleton["walking_left"]);
	m_skeletonAnimations.push_back(m_skeletonWalkLeft);
	m_animationMapSkeleton["walking_right"] = 1;
	m_skeletonWalkRight = Animation(glTexImageTGAFile("../../images/skeleton_walking_right.tga"), 3, 1, 0, "walking_right", m_animationMapSkeleton["walking_right"]);
	m_skeletonAnimations.push_back(m_skeletonWalkRight);
	m_animDefSkeleton = AnimationDef(1, 27, 48, m_skeletonAnimations, m_animationMapSkeleton);
	SetAnimationDef(m_animDefSkeleton);
}

void Skeleton::update(float dt){
	if(x > 800) // TODO REMOVE MAGIC NUMBER 800, USE GAME WORLD CONSTANT
	{
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