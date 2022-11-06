#include"Game.h"
#include<GL/glew.h>
#include<time.h>
#include"Player.h"
#include"StoreClerk.h"
#include"Skeleton.h"
#include"TextString.h"


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

	m_window = SDL_CreateWindow(
	"Frankenstein Test App",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	800, 600,
	SDL_WINDOW_OPENGL);

	if (!m_window)
	{
		fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(m_window);

	// Make sure we have a recent version of OpenGL.
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		fprintf(stderr, "Could not initialize glew. ErrorCode=%s\n", glewGetErrorString(glewError));
		SDL_Quit();
		return 1;
	}
	if (GLEW_VERSION_2_0)
	{
		fprintf(stderr, "OpenGL 2.0 or greater supported: Version=%s\n",
			glGetString(GL_VERSION));
	}
	else
	{
		fprintf(stderr, "OpenGL max supported version is too low.\n");
		SDL_Quit();
		return 1;
	}

	// Setup OpenGL state.
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 800, 600, 0, 0, 100);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand(time(NULL));

	// Load the game characters and other graphics
	LoadData();

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
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(m_glcontext);
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
	m_msPerFrame = m_currentTime - m_previousTime; // ~14 ms
	m_deltaTime = (m_msPerFrame) / 1000.0f; // ~ 0.014
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


	for (int i = 0; i < NUMBER_OF_SKELETONS; i++)
	{
		m_skeletons.at(i)->update(m_deltaTime);
	}

	// TODO FREE MEMORY OF ANY DEAD SPRITES

	//fmod_sys->update(); // If you don't update the sound will play once
}

void Game::GenerateOutput()
{
	// Draw Frame
	glClearColor(255, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this

	m_player->draw();

	for (int i = 0; i < NUMBER_OF_SKELETONS; i++)
	{
		m_skeletons.at(i)->draw();
	}

	m_storeClerk->draw();

	m_textStr->drawText();

	SDL_GL_SwapWindow(m_window);
}

void Game::LoadData()
{
	// Create player
	m_player = new Player(300.0, 64.0, 64, 104, "player", this); //xPos, yPos, player_width, player_height
	m_storeClerk = new StoreClerk(glTexImageTGAFile("../../images/magikarp.tga"), 650, 300, 44, 57);
	m_player->registerObserver(m_storeClerk);
	// Skeleton Creation
	for (int i = 0; i < NUMBER_OF_SKELETONS; i++)
	{
		float xpos = float(rand() % 150 + 50);
		float ypos = float(rand() % 400 + 50);
		m_skeletons.push_back(new Skeleton(xpos, ypos, 27, 48, "skeleton"));
		m_skeletons.at(i)->number = i + 1;
		m_player->registerObserver(m_skeletons.at(i)); // register the skeleton as an observer of the player

			// Test to draw text
		m_textStr = new TextString(this);
		m_textStr->Initialize("Frankenstein!", glTexImageTGAFile("../../images/game_font.tga"), 496, 216, 31, 36, 150, 150);
	}
}

void Game::UnloadData()
{
	delete m_player;
	delete m_storeClerk;

	for (auto skeleton : m_skeletons) { delete skeleton; }
}

