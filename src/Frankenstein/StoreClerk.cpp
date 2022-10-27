#include"StoreClerk.h"

// Required to make a default constructor to avoid compile error C2512

StoreClerk::StoreClerk() : Observer(), Sprite(){
	image = glTexImageTGAFile("../../images/magikarp.tga");
}

StoreClerk::StoreClerk(GLuint i, float xPos, float yPos, int w, int h) : Observer(), image(i), Sprite(xPos, yPos, w, h){}

void StoreClerk::draw(){
	glDrawSprite(image, int(x), int(y), width, height);
}

void StoreClerk::update(float deltaTime){

}

void StoreClerk::respondToObserved(AnimatedSprite* a){
	if(a->getX() > getX() && a->name == "player"){
		printf("Sell, sell, sell!\n");
	}
}