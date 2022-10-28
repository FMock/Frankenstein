#include"Player.h"

Player::Player(){}

Player::Player(float x, float y, int w, int h, const std::string& n) : AnimatedSprite(x, y, w, h, n)
{
	SetupAnimation();
}

Player::Player(float x, float y, int w, int h, AnimationDef& a, const std::string& n) : AnimatedSprite(x, y, w, h, a, n)
{
	SetupAnimation();
}

// Display a Player's data member info as a string
std::string Player::to_string() const {
	std::stringstream oss;
	oss << position.getX() << std::endl
		<< position.getY() << std::endl
		<< width << std::endl
		<< height << std::endl;
	return oss.str();
}

void Player::SetupAnimation()
{
	/*IN THIS GAME, THE PLAYERS FACING DIRECTION IS REPRESENTED AS AN INTEGER*/
	m_animationMap["walking_left"] = 0;
	m_playerWalkLeft = Animation(glTexImageTGAFile("../../images/dwarf_walk_left.tga"), 4, 1, 0, "walking_left", m_animationMap["walking_left"]); //(anim images, frames in animation, rows in animation, start row, name, direction)
	m_playerAnimations.push_back(m_playerWalkLeft);

	m_animationMap["walking_right"] = 1;
	m_playerWalkRight = Animation(glTexImageTGAFile("../../images/dwarf_walk_right.tga"), 4, 1, 0, "walking_right", m_animationMap["walking_right"]);
	m_playerAnimations.push_back(m_playerWalkRight);

	m_animationMap["walking_up"] = 2;
	m_playerWalkUp = Animation(glTexImageTGAFile("../../images/dwarf_walk_up.tga"), 4, 1, 0, "walking_up", m_animationMap["walking_up"]);
	m_playerAnimations.push_back(m_playerWalkUp);

	m_animationMap["walking_down"] = 3;
	m_playerWalkDown = Animation(glTexImageTGAFile("../../images/dwarf_walk_down.tga"), 4, 1, 0, "walking_down", m_animationMap["walking_down"]);
	m_playerAnimations.push_back(m_playerWalkDown);

	m_animationMap["stopped_facing_right"] = 4;
	m_playerStandRight = Animation(glTexImageTGAFile("../../images/dwarf_stand_right.tga"), 1, 1, 0, "stopped_facing_right", m_animationMap["walking_right"]);
	m_playerAnimations.push_back(m_playerStandRight);

	m_animationMap["stopped_facing_left"] = 5;
	m_playerStandLeft = Animation(glTexImageTGAFile("../../images/dwarf_stand_left.tga"), 1, 1, 0, "stopped_facing_left", m_animationMap["walking_left"]);
	m_playerAnimations.push_back(m_playerStandLeft);

	m_animDef = AnimationDef(1, 64, 104, m_playerAnimations, m_animationMap);
	SetAnimationDef(m_animDef);
}
