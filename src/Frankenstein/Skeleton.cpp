#include"Skeleton.h"
#include "Game.h"
#include "TextureManager.h"

using namespace DrawUtilities;

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
	m_skeletonWalkLeft = Animation(TextureManager::instance().load("../../images/skeleton_walking_left.tga"), 3, 1, 0, "walking_left", m_animationMapSkeleton["walking_left"]);
	m_skeletonAnimations.push_back(m_skeletonWalkLeft);
	m_animationMapSkeleton["walking_right"] = 1;
	m_skeletonWalkRight = Animation(TextureManager::instance().load("../../images/skeleton_walking_right.tga"), 3, 1, 0, "walking_right", m_animationMapSkeleton["walking_right"]);
	m_skeletonAnimations.push_back(m_skeletonWalkRight);
	m_animDefSkeleton = AnimationDef(1, 27, 48, m_skeletonAnimations, m_animationMapSkeleton);
	SetAnimationDef(m_animDefSkeleton);
}

void Skeleton::update(float dt){
	// Patrol back and forth, turning around at the world edges.
	float rightBound = (m_game ? static_cast<float>(m_game->getWorldWidth()) : 800.0f) - width;
	if(x > rightBound)
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

	// Shared position/box/animation advance (also fixes the abs() coordinate bug).
	AnimatedSprite::update(dt);
}