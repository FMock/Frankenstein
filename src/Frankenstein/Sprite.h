#pragma once

#include<GL/glew.h>
#include"AABB.h"
#include"Vector2D.h"
#include<cmath>


class Sprite{
public:
	Sprite(void);
	Sprite(float, float, int, int);
	~Sprite(void);
	virtual void draw()=0;
	virtual void update(float)=0;
	float getX()const;
	float getY()const;
	void setX(float);
	void setY(float);
	AABB box;
	AABB& getBox();
	bool AABBIntersect(AABB& box2)const;
protected:
	float x;
	float change_x;
	float y;
	float change_y;
	int width;
	int height;
	Vector2D position;
};
