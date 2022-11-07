#pragma once

#include<GL/glew.h>
#include<vector>
#include"AnimatedSprite.h"
#include<string>
#include<sstream>

using PlayerAnimations = std::vector<Animation>;

class Player : public AnimatedSprite{
public:
	Player();
	Player(float, float, int, int, const std::string&);
	Player(float, float, int, int, const std::string&, class Game* game);
	Player(float, float, int, int, AnimationDef&, const std::string&);
	std::string to_string() const;
	void SetupAnimation() override;
	void update(float);
private:
	Animation m_playerWalkLeft;
	Animation m_playerWalkRight;
	Animation m_playerWalkUp;
	Animation m_playerWalkDown;
	Animation m_playerStandRight;
	Animation m_playerStandLeft;
	PlayerAnimations m_playerAnimations;
	AnimationDef m_animDef;
};
