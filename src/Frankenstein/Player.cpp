#include"Player.h"
#include "Game.h"
#include "SpriteSheetInfo.h"
#include "AnimationParameters.h"

using namespace DrawUtilities;

Player::Player(){}

Player::Player(float x, float y, int w, int h, const std::string& n) : AnimatedSprite(x, y, w, h, n)
{
	SetupAnimation();
}

Player::Player(float x, float y, int w, int h, const std::string& n, Game* game) : AnimatedSprite(x, y, w, h, n, game)
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
	// TODO: LOAD ANIMATION INFO FROM A CONFIG FILE
	SpriteSheetInfo spriteSheetInfo;
	spriteSheetInfo.m_spriteSheetWidth = 256;
	spriteSheetInfo.m_spriteSheetHeight = 520;
	spriteSheetInfo.m_frameWidth = 64;
	spriteSheetInfo.m_frameHeight = 104;

	GLuint texture = glTexImageTGAFile("../../images/dwarf.tga"); // dwarf animation spritesheet

	AnimationParameters animationParams;
	animationParams.image = texture;
	animationParams.framesInAnimation = 4;
	animationParams.rowsInAnimation = 1;
	animationParams.startingRow = 3;
	animationParams.startingCol = 0;
	m_animationMap["walking_left"] = 0;
	m_playerWalkLeft = Animation(animationParams, spriteSheetInfo, "walking_left", m_animationMap["walking_left"]);
	m_playerAnimations.push_back(m_playerWalkLeft);

	m_animationMap["walking_right"] = 1;
	m_playerWalkRight = Animation(texture, 4, 1, 4, 0, spriteSheetInfo, "walking_right", m_animationMap["walking_right"]);
	m_playerAnimations.push_back(m_playerWalkRight);

	m_animationMap["walking_up"] = 2;
	m_playerWalkUp = Animation(texture, 4, 1, 1, 0, spriteSheetInfo, "walking_up", m_animationMap["walking_up"]);
	m_playerAnimations.push_back(m_playerWalkUp);

	m_animationMap["walking_down"] = 3;
	m_playerWalkDown = Animation(texture, 4, 1, 2, 0, spriteSheetInfo, "walking_down", m_animationMap["walking_down"]);
	m_playerAnimations.push_back(m_playerWalkDown);

	m_animationMap["stopped_facing_right"] = 4;
	m_playerStandRight = Animation(texture, 1, 1, 0, 1, spriteSheetInfo, "stopped_facing_right", m_animationMap["walking_right"]);
	m_playerAnimations.push_back(m_playerStandRight);

	m_animationMap["stopped_facing_left"] = 5;
	m_playerStandLeft = Animation(texture, 1, 1, 0, 0, spriteSheetInfo, "stopped_facing_left", m_animationMap["walking_left"]);
	m_playerAnimations.push_back(m_playerStandLeft);

	m_animationMap["stopped_facing_down"] = 6;
	m_playerStandLeft = Animation(texture, 1, 1, 0, 2, spriteSheetInfo, "stopped_facing_down", m_animationMap["walking_down"]);
	m_playerAnimations.push_back(m_playerStandLeft);

	m_animationMap["stopped_facing_up"] = 7;
	m_playerStandLeft = Animation(texture, 1, 1, 0, 3, spriteSheetInfo, "stopped_facing_up", m_animationMap["walking_up"]);
	m_playerAnimations.push_back(m_playerStandLeft);

	m_animDef = AnimationDef(1, 64, 104, m_playerAnimations, m_animationMap);
	SetAnimationDef(m_animDef);
}

void Player::update(float dt)
{
	if (x > 800) // TODO REMOVE MAGIC NUMBER 800, USE GAME WORLD CONSTANT
	{ 
		changeAnimation(animationDef.animationMap["walking_left"]);
		change_x = 0;
		moveLeft();
	}
	else if (x < 0)
	{
		changeAnimation(animationDef.animationMap["walking_right"]);
		change_x = 0;
		moveRight();
	}

	AnimatedSprite::update(dt);
}