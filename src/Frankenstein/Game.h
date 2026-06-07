#pragma once

#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "FrankensteinAPI.h"
#include "Common.h"
#include "Player.h"
#include "StoreClerk.h"
#include "Skeleton.h"
#include "TextString.h"
#include "XmlSettings.h"

const int NUMBER_OF_SKELETONS = 1;

class FRANKENSTEIN_API Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
private:
	SDL_Window* m_window = nullptr;
	SDL_GLContext m_glcontext = nullptr;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	bool LoadData();
	void UnloadData();

    std::unique_ptr<class TextString> m_textStr;

	// The previous frame's keyboard state.
	unsigned char m_kbPrevState[SDL_NUM_SCANCODES] = { 0 };

	// The current frame's keyboard state.
	const unsigned char* m_kbState = NULL;

	// Set this to true to make the game loop exit.
	char m_shouldExit = 0;

	// To regulate frame rate
	unsigned int m_previousTime = 0;
	unsigned int m_currentTime = 0;
	unsigned int m_f_previousTime = 0;
	unsigned int m_f_currentTime = 0;
	float m_deltaTime = 0.0f;
	float m_noKeyPressTime = 0.0f;
	float m_msPerFrame = 0.0f;
	unsigned int m_seconds = 0;
	unsigned int m_fps = 0;


	// Game Objects Specific to TestApp
    std::unique_ptr<Player> m_player;
	std::unique_ptr<StoreClerk> m_storeClerk;
	std::vector<std::unique_ptr<Skeleton>> m_skeletons;

	XmlSettings m_settings;
};