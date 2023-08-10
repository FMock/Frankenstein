#include <iostream>
#include"Player.h"
#include "Game.h"
#include "SpriteSheetInfo.h"
#include "AnimationParameters.h"
#include "Utilities.h"

using namespace DrawUtilities;
using namespace Frankenstein::Utility;

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
	SpriteSheetParameters spriteSheetParams;

	if (!Utilities::ReadXmlFile(R"(../../config/PlayerSpriteSheetInfoParams.xml)", spriteSheetParams))
	{
		std::cout << "There was a problem reading PlayerSpriteSheetInfoParams.xml" << std::endl;
		exit(1);
	}
	std::string spriteSheetParamFile = "../../images/" + spriteSheetParams.m_fileName; // dwarf.tga
	GLuint texture = glTexImageTGAFile(spriteSheetParamFile.c_str());

	auto animationParamVect = std::vector< AnimationParameters>();
	if (!Utilities::ReadCsvFile(R"(../../config/PlayerAnimationParams.csv)", animationParamVect))
	{
		std::cout << "There was a problem reading PlayerAnimationParams.csv" << std::endl;
		exit(1);
	}

	// Load up all player animations
	int count = animationParamVect[1].animationCount;
	for (int i = 0; i < count; i++)
	{
		auto name = animationParamVect[i].animationName;
		m_animationMap[name] = i;
		animationParamVect[i].image = texture;
		m_playerAnimations.push_back(Animation(animationParamVect[i], spriteSheetParams, name, m_animationMap[name]));
	}

	auto frameWidth = spriteSheetParams.m_frameWidth;
	auto frameHeight = spriteSheetParams.m_frameHeight;
	int spritesheetRows = spriteSheetParams.m_spriteSheetHeight / spriteSheetParams.m_frameHeight;
	m_animDef = AnimationDef(spritesheetRows, frameWidth, frameHeight, m_playerAnimations, m_animationMap);
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