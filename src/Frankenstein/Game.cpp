#include "Game.h"
#include <GL/glew.h>
#include <time.h>
#include <memory>
#include "Player.h"
#include "StoreClerk.h"
#include "Skeleton.h"
#include "TextString.h"
#include "TextStringParams.h"
#include "DrawUtils.h"
#include "XmlSettings.h"
#include <direct.h>

Game::Game()
{
}

bool Game::Initialize()
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Could not initialize SDL. ErrorCode=%s\n", SDL_GetError());
		return false;
	}

	// Create the window and OpenGL context.
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);



	printf("Current working directory: %s\n", _getcwd(NULL, 0));
   if (!m_settings.Load("../../config/Game-Settings.xml")) {
	  fprintf(stderr, "Could not load settings file ../../config/Game-Settings.xml\n");
	  return false;
   }
   int winWidth = m_settings.GetInt("Window", "Width");
   int winHeight = m_settings.GetInt("Window", "Height");
   std::string winTitle = m_settings.GetString("Window", "Title");
   if (winWidth == 0 || winHeight == 0 || winTitle.empty()) {
	  fprintf(stderr, "Missing required window settings in XML.\n");
	  return false;
   }
   m_window = SDL_CreateWindow(
	  winTitle.c_str(),
	  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	  winWidth, winHeight,
	  SDL_WINDOW_OPENGL);

	if (!m_window)
	{
		fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
		return false;
	}

	// Create an OpenGL context associated with the window.
	m_glcontext = SDL_GL_CreateContext(m_window);
	if (!m_glcontext)
	{
		fprintf(stderr, "Could not create OpenGL context. ErrorCode=%s\n", SDL_GetError());
		return false;
	}

	// Make sure we have a recent version of OpenGL.
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		fprintf(stderr, "Could not initialize glew. ErrorCode=%s\n", glewGetErrorString(glewError));
		return false;
	}
	if (GLEW_VERSION_2_0)
	{
		fprintf(stderr, "OpenGL 2.0 or greater supported: Version = %s\n",
			glGetString(GL_VERSION));
	}
	else
	{
		fprintf(stderr, "OpenGL max supported version is too low.\n");
		return false;
	}

	// Setup OpenGL state.
   glViewport(0, 0, winWidth, winHeight);
   glMatrixMode(GL_PROJECTION);
   glOrtho(0, winWidth, winHeight, 0, 0, 100);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   srand(static_cast<unsigned int>(time(NULL)));

	// Load the game characters and other graphics
	if (!LoadData())
		return false;

	return true;
}

void Game::RunLoop()
{
	while (!m_shouldExit)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	UnloadData();
	// Tear down in reverse order of creation, guarding against a partially
	// initialized Game (Shutdown is called even when Initialize() failed).
	if (m_glcontext)
	{
		// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
		SDL_GL_DeleteContext(m_glcontext);
		m_glcontext = nullptr;
	}
	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
	SDL_Quit();
}

void Game::ProcessInput()
{
	m_kbState = SDL_GetKeyboardState(NULL);// We want status of all the keys

	memcpy(m_kbPrevState, m_kbState, sizeof(m_kbPrevState)); // copy key states into previous key states array

	// Handle OS message pump.
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_shouldExit = 1;
		}
	}

	// Take action if any keys are pressed. check SDL SCANCODES https://wiki.libsdl.org/SDL_Scancode
	if (m_kbState[SDL_SCANCODE_BACKSPACE] && !m_kbPrevState[SDL_SCANCODE_BACKSPACE])
	{
		m_noKeyPressTime = 0;
		printf("BACKSPACE\n");
	}
	else if (m_kbState[SDL_SCANCODE_RETURN] && !m_kbPrevState[SDL_SCANCODE_RETURN])
	{
		m_noKeyPressTime = 0;
		//printf("RETURN\n");
		//player.getRotate() ? player.setRotate(false) : player.setRotate(true);
	}
	else if (m_kbState[SDL_SCANCODE_LEFT] && !m_kbPrevState[SDL_SCANCODE_LEFT])
	{ // LEFT ARROW KEY PRESSED
		m_noKeyPressTime = 0;
		if (m_player->getCurrentAnimation() != m_player->animationDef.animationMap["walking_left"])
			m_player->changeAnimation(m_player->animationDef.animationMap["walking_left"]);
		m_player->moveLeft();
	}// If no directional keys are being pressed, stop the player's movement
	else if (!m_kbState[SDL_SCANCODE_LEFT] &&									// NO DIRECTIONAL KEYS PRESSED
		!m_kbPrevState[SDL_SCANCODE_LEFT] &&
		!m_kbState[SDL_SCANCODE_RIGHT] &&
		!m_kbPrevState[SDL_SCANCODE_RIGHT] &&
		!m_kbState[SDL_SCANCODE_UP] &&
		!m_kbPrevState[SDL_SCANCODE_UP] &&
		!m_kbState[SDL_SCANCODE_DOWN] &&
		!m_kbPrevState[SDL_SCANCODE_DOWN])
	{
		m_noKeyPressTime = 0;
		if (m_player->getFacingDirection() == m_player->RIGHT)
			m_player->changeAnimation(m_player->animationDef.animationMap["stopped_facing_right"]);
		else if (m_player->getFacingDirection() == m_player->LEFT)
			m_player->changeAnimation(m_player->animationDef.animationMap["stopped_facing_left"]);
		else if (m_player->getFacingDirection() == m_player->DOWN)
			m_player->changeAnimation(m_player->animationDef.animationMap["stopped_facing_down"]);
		else if (m_player->getFacingDirection() == m_player->UP)
			m_player->changeAnimation(m_player->animationDef.animationMap["stopped_facing_up"]);
		m_player->stop();
	}
	else if (m_kbState[SDL_SCANCODE_RIGHT] && !m_kbPrevState[SDL_SCANCODE_RIGHT])
	{  // RIGHT ARROW KEY PRESSED
		m_noKeyPressTime = 0;
		if (m_player->getCurrentAnimation() != m_player->animationDef.animationMap["walking_right"])
			m_player->changeAnimation(m_player->animationDef.animationMap["walking_right"]);

		m_player->moveRight();
	}
	else if (m_kbState[SDL_SCANCODE_UP] && !m_kbPrevState[SDL_SCANCODE_UP])
	{  // UP ARROW KEY PRESSED
		m_noKeyPressTime = 0;
		if (m_player->getCurrentAnimation() != m_player->animationDef.animationMap["walking_up"])
			m_player->changeAnimation(m_player->animationDef.animationMap["walking_up"]);
		m_player->moveUp();
	}
	else if (m_kbState[SDL_SCANCODE_DOWN] && !m_kbPrevState[SDL_SCANCODE_DOWN])
	{  // DOWN ARROW KEY PRESSED
		m_noKeyPressTime = 0;
		if (m_player->getCurrentAnimation() != m_player->animationDef.animationMap["walking_down"])
			m_player->changeAnimation(m_player->animationDef.animationMap["walking_down"]);
		m_player->moveDown();
	}
	else if (m_kbState[SDL_SCANCODE_SPACE] && !m_kbPrevState[SDL_SCANCODE_SPACE])
	{
		m_noKeyPressTime = 0;
		printf("SPACE\n");
	}
	// player wants to exit game
	if (m_kbState[SDL_SCANCODE_ESCAPE])
	{
		m_shouldExit = 1;
	}
}

void Game::UpdateGame()
{
	// Compute deltaTime - the time difference between each frame
	m_currentTime = SDL_GetTicks();
   m_msPerFrame = static_cast<float>(m_currentTime - m_previousTime); // ~14 ms
   m_deltaTime = m_msPerFrame / 1000.0f; // ~ 0.014
	m_previousTime = m_currentTime;

	// Calculate FPS and print
	m_f_currentTime = SDL_GetTicks();
	if (m_f_currentTime > m_f_previousTime + 1000)
	{
		m_seconds++;
		//printf("fps: %i\n", fps);
		m_fps = 0;
		m_f_previousTime = m_f_currentTime;
	}

	m_fps++; // increment frame counter each iteration

   m_player->update(m_deltaTime);

   for (auto& skeleton : m_skeletons)
   {
	   skeleton->update(m_deltaTime);
   }

	// TODO FREE MEMORY OF ANY DEAD SPRITES

	//fmod_sys->update(); // If you don't update the sound will play once
}

void Game::GenerateOutput()
{
	// Draw Frame
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this


   m_player->draw();

   for (auto& skeleton : m_skeletons)
   {
	   skeleton->draw();
   }

   m_storeClerk->draw();

   m_textStr->drawText();

	SDL_GL_SwapWindow(m_window);
}

bool Game::LoadData()
{
   // Player
   float playerX = static_cast<float>(m_settings.GetInt("Player", "X"));
   float playerY = static_cast<float>(m_settings.GetInt("Player", "Y"));
   int playerW = m_settings.GetInt("Player", "Width");
   int playerH = m_settings.GetInt("Player", "Height");
   std::string playerName = m_settings.GetString("Player", "Name");
   if (playerW == 0 || playerH == 0 || playerName.empty()) {
	  fprintf(stderr, "Missing required player settings in XML.\n");
	  return false;
   }
   m_player = std::make_unique<Player>(playerX, playerY, playerW, playerH, playerName, this);

   // StoreClerk
   std::string clerkImg = m_settings.GetString("StoreClerk", "Image");
   int clerkX = m_settings.GetInt("StoreClerk", "X");
   int clerkY = m_settings.GetInt("StoreClerk", "Y");
   int clerkW = m_settings.GetInt("StoreClerk", "Width");
   int clerkH = m_settings.GetInt("StoreClerk", "Height");
   if (clerkImg.empty() || clerkW == 0 || clerkH == 0) {
	  fprintf(stderr, "Missing required store clerk settings in XML.\n");
	  return false;
   }
   m_storeClerk = std::make_unique<StoreClerk>(DrawUtilities::glTexImageTGAFile(clerkImg.c_str()), static_cast<float>(clerkX), static_cast<float>(clerkY), clerkW, clerkH);
   m_player->registerObserver(m_storeClerk.get());

   // Skeletons
   int skeletonCount = m_settings.GetInt("Skeleton", "Count");
   int skeletonXMin = m_settings.GetInt("Skeleton", "XMin");
   int skeletonXMax = m_settings.GetInt("Skeleton", "XMax");
   int skeletonYMin = m_settings.GetInt("Skeleton", "YMin");
   int skeletonYMax = m_settings.GetInt("Skeleton", "YMax");
   int skeletonW = m_settings.GetInt("Skeleton", "Width");
   int skeletonH = m_settings.GetInt("Skeleton", "Height");
   std::string skeletonName = m_settings.GetString("Skeleton", "Name");
   if (skeletonCount == 0 || skeletonW == 0 || skeletonH == 0 || skeletonName.empty()) {
	  fprintf(stderr, "Missing required skeleton settings in XML.\n");
	  return false;
   }
   for (int i = 0; i < skeletonCount; i++) {
	  float xpos = static_cast<float>(rand() % (skeletonXMax - skeletonXMin + 1) + skeletonXMin);
	  float ypos = static_cast<float>(rand() % (skeletonYMax - skeletonYMin + 1) + skeletonYMin);
	  auto skeleton = std::make_unique<Skeleton>(xpos, ypos, skeletonW, skeletonH, skeletonName);
	  skeleton->number = i + 1;
	  m_player->registerObserver(skeleton.get());
	  m_skeletons.push_back(std::move(skeleton));
   }

   // Font/TextString
   m_textStr = std::make_unique<TextString>(this);
   TextStringInitParams params;
   std::string fontImg = m_settings.GetString("Font", "Image");
   if (fontImg.empty()) {
	  fprintf(stderr, "Missing required font image setting in XML.\n");
	  return false;
   }
   params.image = DrawUtilities::glTexImageTGAFile(fontImg.c_str());
   params.imageWidth = m_settings.GetInt("Font", "ImageWidth");
   params.imageHeight = m_settings.GetInt("Font", "ImageHeight");
   params.frameWidth = m_settings.GetInt("Font", "FrameWidth");
   params.frameHeight = m_settings.GetInt("Font", "FrameHeight");
   params.x = m_settings.GetInt("Font", "X");
   params.y = m_settings.GetInt("Font", "Y");
   std::string fontText = m_settings.GetString("Font", "Text");
   if (params.imageWidth == 0 || params.imageHeight == 0 || params.frameWidth == 0 || params.frameHeight == 0 || fontText.empty()) {
	  fprintf(stderr, "Missing required font settings in XML.\n");
	  return false;
   }
   m_textStr->Initialize(fontText.c_str(), params);

   return true;
}

void Game::UnloadData()
{
   m_player.reset();
   m_storeClerk.reset();
   m_skeletons.clear();
   m_textStr.reset();
}

