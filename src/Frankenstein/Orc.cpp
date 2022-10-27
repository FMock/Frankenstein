#include"Orc.h"

// Required to make a default constructor to avoid compile error C2512

Orc::Orc() : Observer(), Sprite(){
	image = glTexImageTGAFile("../../images/ord_small_60w_75h.tga");
}

Orc::Orc(GLuint i, float xPos, float yPos, int w, int h) : Observer(), image(i), Sprite(xPos, yPos, w, h){}

void Orc::draw(){
	glDrawSprite(image, int(x), int(y), width, height);
}

void Orc::update(float deltaTime){

}

void Orc::respondToObserved(AnimatedSprite* a){
	/*
	if(a->getX() <  getX() && a->name == "player"){
		printf("Orc number %i Attacks\n", number);
	}
	*/
}
