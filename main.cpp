#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<GL/glew.h>
#include<stdio.h>
#include<string>
#include<iostream>
#include<assert.h>
#include"DrawUtils.h"
#include<vector>
#include<time.h>
#include"Player.h"
#include"Orc.h"

/*  main.cpp
	Project Name - Frankenstein - A C++ game engine for simple 2D games
	Program Name - main.cpp
	Author - Frank Mock
	Project Start Date - 2/6/2018 */

// Set this to true to make the game loop exit.
char shouldExit = 0;

// The previous frame's keyboard state.
unsigned char kbPrevState[SDL_NUM_SCANCODES] = { 0 };

// The current frame's keyboard state.
const unsigned char* kbState = NULL;

// To regulate frame rate
unsigned int previousTime = 0;
unsigned int currentTime = 0;
unsigned int f_previousTime = 0;
unsigned int f_currentTime = 0;
float deltaTime = 0.0f;
float noKeyPressTime = 0.0f;
float msPerFrame = 0.0f;
unsigned int seconds = 0;
unsigned int fps = 0;

// List to hold the Orcs
std::vector<Orc *> orcs = std::vector<Orc *>();

// Sprite Objects
Player player;
Orc orc;

// Function Prototyes
void processInputs(float);
void update(float);
void draw();

int main(void)
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not initialize SDL. ErrorCode=%s\n", SDL_GetError());
		return 1;
	}

	// Create the window and OpenGL context.
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* window = SDL_CreateWindow(
		"Sprite and Vector Project",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600,
		SDL_WINDOW_OPENGL);
	if (!window) {
		fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_GL_CreateContext(window);

	// Make sure we have a recent version of OpenGL.
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Could not initialize glew. ErrorCode=%s\n", glewGetErrorString(glewError));
		SDL_Quit();
		return 1;
	}
	if (GLEW_VERSION_2_0) {
		fprintf(stderr, "OpenGL 2.0 or greater supported: Version=%s\n",
			glGetString(GL_VERSION));
	}
	else {
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

	srand (time(NULL));

	// Create the player and enemy objects (GL image, xPosition, yPosition, width, height)
	//player = Player(glTexImageTGAFile("images/magikarp.tga"), 300.0, 64.0, 44, 57);
	player = Player(glTexImageTGAFile("images/run_cycle.tga"), 300.0, 64.0, 1536, 128);

	// Dynamically create orc enemies and place in vector
	for(int i = 0; i < 4; i++){
		float xpos = float(rand() % 100 + 50);
		float ypos = float(rand() % 400 + 50);
		orcs.push_back(new Orc(glTexImageTGAFile("images/skeleton.tga"), xpos, ypos, 27, 48));
		player.registerObserver(orcs.at(i)); // register the Orc as an observer of the player
	}


	//********** GAME LOOP *************************************************************
	while (!shouldExit) {

		kbState = SDL_GetKeyboardState(NULL);// We want status of all the keys

		// Compute deltaTime - the time difference between each frame
		currentTime = SDL_GetTicks();
		msPerFrame = currentTime - previousTime; // ~14 ms
		deltaTime = (msPerFrame) / 1000.0f; // ~ 0.014
		previousTime = currentTime;

		// Calculate FPS and print
		f_currentTime = SDL_GetTicks();
		if (f_currentTime > f_previousTime + 1000) {
			seconds++;
			//printf("fps: %i\n", fps);
			fps = 0;
			f_previousTime = f_currentTime;
		}
		
		fps++; // increment frame counter each iteration

		assert(glGetError() == GL_NO_ERROR);

		// ************* Process Player Inputs  ****************************************
		processInputs(deltaTime);

		// ************* Update Objects  ************************************************
		update(deltaTime); //update game objects
		
		// *********** Draw Frame *******************************************************
		glClearColor(0, 0, 0, 1);  
		glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this
		draw(); // draw game objects

		//*********** Troubleshooting *************************************************
		//printf("Current Key: %s\n", KeyInfo::getCurrentKey().c_str());
		//printf("Previous Key: %s\n", KeyInfo::getPrevKey().c_str());
		//printf("X = %f\n", player.getX());
		printf("%s\n", player.to_string().c_str());
		//printf("kbState[SDL_SCANCODE_LEFT] = %i\n", kbState[SDL_SCANCODE_LEFT]);
		//printf("kbPrevState[SDL_SCANCODE_LEFT] = %i\n", kbPrevState[SDL_SCANCODE_LEFT]);
		
		// Present the most recent frame.
		SDL_GL_SwapWindow(window);

	} //***** END GAME LOOP ********************************************************

	SDL_Quit();
	return 0;
}

void processInputs(float dt){
	memcpy(kbPrevState, kbState, sizeof(kbPrevState)); // copy key states into previous key states array
		
	// Handle OS message pump.
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			shouldExit = 1;
		}
	}

	// Take action if any keys are pressed. check SDL SCANCODES https://wiki.libsdl.org/SDL_Scancode
	if(kbState[SDL_SCANCODE_BACKSPACE] && !kbPrevState[SDL_SCANCODE_BACKSPACE]){
		noKeyPressTime = 0;
		printf("BACKSPACE\n");
	}
	else if(kbState[SDL_SCANCODE_RETURN] && !kbPrevState[SDL_SCANCODE_RETURN]){
		noKeyPressTime = 0;
		//printf("RETURN\n");
		player.getRotate() ? player.setRotate(false) : player.setRotate(true);
	}
	else if(kbState[SDL_SCANCODE_LEFT] && !kbPrevState[SDL_SCANCODE_LEFT]){
		noKeyPressTime = 0;
		player.moveLeft();
	}// If no directional keys are being pressed, stop the player's movement
	else if(!kbState[SDL_SCANCODE_LEFT] && 
			!kbPrevState[SDL_SCANCODE_LEFT] &&
			!kbState[SDL_SCANCODE_RIGHT] && 
			!kbPrevState[SDL_SCANCODE_RIGHT] && 
			!kbState[SDL_SCANCODE_UP] && 
			!kbPrevState[SDL_SCANCODE_UP] && 
			!kbState[SDL_SCANCODE_DOWN] &&
			!kbPrevState[SDL_SCANCODE_DOWN]){
		noKeyPressTime = 0;
		player.stop();
	}
	else if(kbState[SDL_SCANCODE_RIGHT] && !kbPrevState[SDL_SCANCODE_RIGHT]){
		noKeyPressTime = 0;
		player.moveRight();
	}
	else if(kbState[SDL_SCANCODE_UP] && !kbPrevState[SDL_SCANCODE_UP]){
		noKeyPressTime = 0;
		player.moveUp();
	}
	else if(kbState[SDL_SCANCODE_DOWN] && !kbPrevState[SDL_SCANCODE_DOWN]){
		noKeyPressTime = 0;
		player.moveDown();
	}
	else if(kbState[SDL_SCANCODE_SPACE] && !kbPrevState[SDL_SCANCODE_SPACE]){
		noKeyPressTime = 0;
		printf("SPACE\n");
	}
	// player wants to exit game
	if (kbState[SDL_SCANCODE_ESCAPE]) {
		shouldExit = 1;
	}
}

void update(float dt){
	player.update(dt);
	//fmod_sys->update(); // If you don't update the sound will play once
}

// Render graphics
void draw(){
	player.draw();

	for(int i = 0; i < 4; i++){
		orcs.at(i)->draw();
	}
}

/*
* Checks AABB/AABB collisions
* Used to determine if a TextBlock has touched another TextBlock

bool AABBIntersect(AABB box1, AABB box2){
	//box1 to the right
	if(box1.x > box2.x + box2.w){
		return false;
	}

	// box1 top the left
	if(box1.x + box1.w < box2.x){
		return false;
	}

	// box1 below
	if(box1.y > box2.y + box2.h){
		return false;
	}

	// box1 above
	if(box1.y + box1.h < box2.y){
		return false;
	}
	
	return true;
}


// Takes a numeric string and returns its integer representation
int string_to_int(std::string s){
	std::istringstream instr(s);
	int n;
	instr >> n;
	return n;
}

// convert an integer to a string. return the string
std::string int_to_string(int n){
	ostringstream oss;
	oss << n;
	return oss.str();
}
*/
