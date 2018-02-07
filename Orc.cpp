#include"Orc.h"

// Required to make a default constructor to avoid compile error C2512
Orc::Orc():Observer(), Sprite(){
	image = glTexImageTGAFile("images/skeleton.tga");
}

Orc::Orc(GLuint i, float xPos, float yPos, int w, int h):image(i), Observer(), Sprite(xPos, yPos, w, h){}

void Orc::notify(Player* player){
	if(player->getX() < x + 50){
		//printf("Orc says, ATTACK!\n");
		//player->getRotate() ? player->setRotate(true):player->setRotate(true);
	}
}

void Orc::draw(){
	glDrawSprite(image, int(x), int(y), width, height);
}

void Orc::update(float deltaTime){

}