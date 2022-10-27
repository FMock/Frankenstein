#include"Player.h"

Player::Player(){}

Player::Player(float x, float y, int w, int h, AnimationDef& a, const std::string& n) : AnimatedSprite(x, y, w, h, a, n){}

// Display a Player's data member info as a string
std::string Player::to_string() const {
	std::stringstream oss;
	oss << position.getX() << std::endl
		<< position.getY() << std::endl
		<< width << std::endl
		<< height << std::endl;
	return oss.str();
}