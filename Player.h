#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include<GL/glew.h>
#include<vector>
#include"AnimatedSprite.h"
#include<string>
#include<sstream>

class Player : public AnimatedSprite{
public:
	Player();
	Player(float, float, int, int, AnimationDef&, const std::string&);
	std::string Player::to_string() const;
};

#endif