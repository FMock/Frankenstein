#include"Vector2D.h"

Vector2D::Vector2D(){};
Vector2D::Vector2D(float xPos = 0, float yPos = 0) : x(xPos), y(yPos){}

// Overload equality operator
// Returns true if both vectors contain same x and y, false otherwise
bool Vector2D::operator ==(const Vector2D& v) const{
	return x == v.x && y == v.y;
}

// Overload the inequality operator
// returns true if lvalue and rvalue members are not the same
bool Vector2D::operator !=(const Vector2D& v) const{
	return x != v.x || y != v.y;
}

// Overload assignment operartor
// Returns reference to the lvalue
Vector2D& Vector2D::operator =(const Vector2D& v){
	x = v.x;
	y = v.y;
	return *this;
}

// Returns negated vector
Vector2D Vector2D::operator -() const{
	return Vector2D(-x, -y);
}

// Zero a vector
void Vector2D::zero(){
	x = 0.0f;
	y = 0.0f;
}

// Subtract vectors
Vector2D Vector2D::operator -(const Vector2D& v) const{
	return Vector2D(x - v.x, y - v.y);
}

// Add Vectors
Vector2D Vector2D::operator +(const Vector2D& v) const{
	return Vector2D(x + v.x, y + v.y);
}

// Multiply vector by a scalar
Vector2D Vector2D::operator*(float f) const{
	return Vector2D(x * f, y * f);
}

// Divide vector by scalar
Vector2D Vector2D::operator /(float f) const{
	// <TO DO: Check for divide by zero>
	float d = 1.0f/f;
	return Vector2D(x * d, y * d);
}

// Another divide option
Vector2D Vector2D::divideBy(float divisor) const{
	return Vector2D(x/divisor, y/divisor);
}

// Overload +=
Vector2D& Vector2D::operator +=(const Vector2D& v){
	x += v.x;
	y += v.y;
	return *this;
}

//Overload -=
Vector2D& Vector2D::operator -=(const Vector2D& v){
	x -= v.x;
	y -= v.y;
	return *this;
}

// return the length of this vector
float Vector2D::magnitude() const{
	return sqrt(x*x + y*y);
}

// Turn this vector into a unit vector
void Vector2D::normalize(){
	float lengthSq = x*x + y*y;
	if(lengthSq > 0){
		float oneOverLength = 1.0f / sqrt(lengthSq);
		x *= oneOverLength;
		y *= oneOverLength;
	}
}

float Vector2D::operator *(const Vector2D& v) const{
	return x*v.x + y*v.y;
}