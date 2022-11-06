#ifndef SKELETON_H
#define SKELETON_H

#include"Observer.h"
#include"AnimatedSprite.h"

using SkeletonAnimations = std::vector<Animation>;

class Skeleton : public Observer, public AnimatedSprite{
public:
	inline Skeleton(){}
	Skeleton(float, float, int, int, const std::string&);
	Skeleton(float, float, int, int, const std::string&, class Game* game);
	Skeleton(float, float, int, int, AnimationDef&, const std::string&);
	int number;
	void update(float);
	virtual void respondToObserved(AnimatedSprite*);
	void SetupAnimation() override;

private:
	Animation m_skeletonWalkLeft;
	Animation m_skeletonWalkRight;
	AnimationDef m_animDefSkeleton;
	std::map<std::string, int> m_animationMapSkeleton;
	SkeletonAnimations m_skeletonAnimations;
};

#endif