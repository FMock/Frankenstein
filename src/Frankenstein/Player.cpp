#include <iostream>
#include <stdexcept>
#include"Player.h"
#include "Game.h"
#include "SpriteSheetInfo.h"
#include "AnimationParameters.h"
#include "Utilities.h"
#include "TextureManager.h"

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
		throw std::runtime_error("Could not read ../../config/PlayerSpriteSheetInfoParams.xml");
	}
	std::string spriteSheetParamFile = "../../images/" + spriteSheetParams.m_fileName; // dwarf.tga
	GLuint texture = TextureManager::instance().load(spriteSheetParamFile);

	auto animationParamVect = std::vector< AnimationParameters>();
	if (!Utilities::ReadCsvFile(R"(../../config/PlayerAnimationParams.csv)", animationParamVect))
	{
		throw std::runtime_error("Could not read ../../config/PlayerAnimationParams.csv");
	}

	// Load up all player animations (one per row parsed from the CSV)
	if (animationParamVect.empty())
	{
		throw std::runtime_error("PlayerAnimationParams.csv contained no animation rows");
	}
	int count = static_cast<int>(animationParamVect.size());
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
	AnimatedSprite::update(dt);

	// Stop at the world edges (the camera scrolls to follow; no bounce-back).
	float maxX = (m_game ? static_cast<float>(m_game->getWorldWidth()) : 800.0f) - width;
	float maxY = (m_game ? static_cast<float>(m_game->getWorldHeight()) : 600.0f) - height;
	if (x < 0)    { x = 0.0f;  change_x = 0; }
	if (x > maxX) { x = maxX;  change_x = 0; }
	if (y < 0)    { y = 0.0f;  change_y = 0; }
	if (y > maxY) { y = maxY;  change_y = 0; }
	setX(x);
	setY(y);
}