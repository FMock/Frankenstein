#ifndef SKELETON_H
#define SKELETON_H

#include"Observer.h"
#include"AnimatedSprite.h"

class Skeleton : public Observer, public AnimatedSprite{
public:
	inline Skeleton(){}
	Skeleton(float, float, int, int, AnimationDef&, const std::string&);
	int number;
	void update(float);
	virtual void respondToObserved(AnimatedSprite*);
};

#endif