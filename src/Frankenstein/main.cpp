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
#include"AnimatedSprite.h"
#include"animation.h"
#include"AnimationDef.h"
#include"Orc.h"
#include"StoreClerk.h"
#include<map>
#include"Skeleton.h"
#include"TextString.h"

/*  main.cpp
	Project Name - Frankenstein
	Program Name - main.cpp
	Author - Frank Mock
	Start Date  - 1/27/2018 
	Last update - 10/28/2022
*/

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


// Sprite Objects
Player player;
StoreClerk storeClerk;
// text object
TextString textStr;

// Player Setup
Player player2;

// Skeleton Setup
std::vector<Skeleton*> skeletons = std::vector<Skeleton*>();
const int NUMBER_OF_SKELETONS = 1;
Skeleton skeleton;

// Function Prototyes
void processInputs(float);
void update(float);
void draw();

/* The following enum represent a player animation
 * Be sure that these are entered in the same order that the animations are entered in vector
 * This enum is used mainly for better readability*/
enum directions{LEFT, RIGHT, UP, DOWN, STOPPED_FACE_RIGHT, STOPPED_FACE_LEFT};

int main(void)
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		fprintf(stderr, "Could not initialize SDL. ErrorCode=%s\n", SDL_GetError());
		return 1;
	}

	// Create the window and OpenGL context.
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow(
		"Frankenstein Test App",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600,
		SDL_WINDOW_OPENGL);

	if (!window) 
	{
		fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_GL_CreateContext(window);

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

	srand (time(NULL));

	// Create player
	player2 = Player(300.0, 64.0, 64, 104, "player"); //xPos, yPos, player_width, player_height

	storeClerk = StoreClerk(glTexImageTGAFile("../../images/magikarp.tga"), 650, 300, 44, 57);
	player2.registerObserver(&storeClerk);

	// Skeleton Creation
	for(int i = 0; i < NUMBER_OF_SKELETONS; i++)
	{
		float xpos = float(rand() % 150 + 50);
		float ypos = float(rand() % 400 + 50);
		skeletons.push_back(new Skeleton(xpos, ypos, 27, 48, "skeleton"));
		skeletons.at(i)->number = i + 1;
		player2.registerObserver(skeletons.at(i)); // register the skeleton as an observer of the player
	}

	// Test to draw text
	textStr = TextString();
	textStr.Initialize("Frankenstein!", glTexImageTGAFile("../../images/game_font.tga"), 496, 216, 31, 36, 150, 150);


	// GAME LOOP
	while (!shouldExit) 
	{
		kbState = SDL_GetKeyboardState(NULL);// We want status of all the keys

		// Compute deltaTime - the time difference between each frame
		currentTime = SDL_GetTicks();
		msPerFrame = currentTime - previousTime; // ~14 ms
		deltaTime = (msPerFrame) / 1000.0f; // ~ 0.014
		previousTime = currentTime;

		// Calculate FPS and print
		f_currentTime = SDL_GetTicks();
		if (f_currentTime > f_previousTime + 1000) 
		{
			seconds++;
			//printf("fps: %i\n", fps);
			fps = 0;
			f_previousTime = f_currentTime;
		}
		
		fps++; // increment frame counter each iteration

		assert(glGetError() == GL_NO_ERROR);

		// Process Player Inputs
		processInputs(deltaTime);

		// Update Objects
		update(deltaTime); //update game objects
		
		// Draw Frame
		glClearColor(0, 0, 0, 1);  
		glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this
		draw(); // draw game objects

		//*********** Troubleshooting *************************************************
		//printf("Current Key: %s\n", KeyInfo::getCurrentKey().c_str());
		//printf("Previous Key: %s\n", KeyInfo::getPrevKey().c_str());
		//printf("X = %f\n", player.getX());
		//printf("%s\n", player2.to_string().c_str());
		//printf("kbState[SDL_SCANCODE_LEFT] = %i\n", kbState[SDL_SCANCODE_LEFT]);
		//printf("kbPrevState[SDL_SCANCODE_LEFT] = %i\n", kbPrevState[SDL_SCANCODE_LEFT]);
		/*
		for(int i = 0; i < NUMBER_OF_SKELETONS; i++){
			printf("%s\n", skeletons.at(i)->to_string().c_str());
		}
		*/

		
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
		//player.getRotate() ? player.setRotate(false) : player.setRotate(true);
	}
	else if(kbState[SDL_SCANCODE_LEFT] && !kbPrevState[SDL_SCANCODE_LEFT]){ // LEFT ARROW KEY PRESSED
		noKeyPressTime = 0;
		if(player2.getCurrentAnimation() != player2.animationDef.animationMap["walking_left"])
			player2.changeAnimation(player2.animationDef.animationMap["walking_left"]);
		player2.moveLeft();
	}// If no directional keys are being pressed, stop the player's movement
	else if(!kbState[SDL_SCANCODE_LEFT] &&									// NO DIRECTIONAL KEYS PRESSED
			!kbPrevState[SDL_SCANCODE_LEFT] &&
			!kbState[SDL_SCANCODE_RIGHT] && 
			!kbPrevState[SDL_SCANCODE_RIGHT] && 
			!kbState[SDL_SCANCODE_UP] && 
			!kbPrevState[SDL_SCANCODE_UP] && 
			!kbState[SDL_SCANCODE_DOWN] &&
			!kbPrevState[SDL_SCANCODE_DOWN]){
		noKeyPressTime = 0;
		if(player2.getFacingDirection() == player2.RIGHT)
			player2.changeAnimation(player2.animationDef.animationMap["stopped_facing_right"]);
		else if(player2.getFacingDirection() == player2.LEFT)
			player2.changeAnimation(player2.animationDef.animationMap["stopped_facing_left"]);
		player2.stop();
	}
	else if(kbState[SDL_SCANCODE_RIGHT] && !kbPrevState[SDL_SCANCODE_RIGHT]){  // RIGHT ARROW KEY PRESSED
		noKeyPressTime = 0;
		if(player2.getCurrentAnimation() != player2.animationDef.animationMap["walking_right"])
			player2.changeAnimation(player2.animationDef.animationMap["walking_right"]);

		player2.moveRight();
	}
	else if(kbState[SDL_SCANCODE_UP] && !kbPrevState[SDL_SCANCODE_UP]){  // UP ARROW KEY PRESSED
		noKeyPressTime = 0;
		if(player2.getCurrentAnimation() != player2.animationDef.animationMap["walking_up"])
			player2.changeAnimation(player2.animationDef.animationMap["walking_up"]);
		player2.moveUp();
	}
	else if(kbState[SDL_SCANCODE_DOWN] && !kbPrevState[SDL_SCANCODE_DOWN]){  // DOWN ARROW KEY PRESSED
		noKeyPressTime = 0;
		if(player2.getCurrentAnimation() != player2.animationDef.animationMap["walking_down"])
			player2.changeAnimation(player2.animationDef.animationMap["walking_down"]);
		player2.moveDown();
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
	player2.update(dt);

	
	for(int i = 0; i < NUMBER_OF_SKELETONS; i++){
		skeletons.at(i)->update(dt);
	}
	
	//fmod_sys->update(); // If you don't update the sound will play once
}

// Render graphics
void draw(){
	player2.draw();

	for(int i = 0; i < NUMBER_OF_SKELETONS; i++){
		skeletons.at(i)->draw();
	}

	storeClerk.draw();

	textStr.drawText();
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
