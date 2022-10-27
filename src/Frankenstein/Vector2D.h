#pragma once
#ifndef VECTOR2D_H
#define VECTOR2D_H

#include<math.h>

class Vector2D{
private:
	float x;
	float y;

public:
	Vector2D();
	Vector2D(float xPos, float yPos);
	// Copy constructor
	Vector2D(const Vector2D& v) : x(v.x), y(v.y){}
	inline float getX() const { return x;}
	inline float getY() const { return y;}
	inline void setX(float newX) {x = newX; }
	inline void setY(float newY) { y = newY; }
	bool operator ==(const Vector2D&) const;
	bool operator !=(const Vector2D&) const;
	Vector2D& operator =(const Vector2D&);
	void zero();
	Vector2D operator -() const;
	Vector2D operator -(const Vector2D&) const;
	Vector2D operator +(const Vector2D&) const;
	Vector2D operator *(float) const;
	Vector2D operator /(float) const;
	Vector2D divideBy(float) const;
	Vector2D& operator +=(const Vector2D&);
	Vector2D& operator -=(const Vector2D&);
	float magnitude() const;
	void normalize();
	float operator *(const Vector2D&) const;
};

#endif