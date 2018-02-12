#include"Player.h"

// Required to make a default constructor to avoid compile error C2512
Player::Player() : Sprite(), position(Vector2D()){
	image = glTexImageTGAFile("images/magikarp.tga");
	speed_x = 50.0;
}

Player::Player(GLuint i, float xPos, float yPos, int w, int h):image(i), speed_x(50.0), speed_y(50.0), rotate(false), rotateAngle(0.0), Sprite(xPos, yPos, w, h), position(Vector2D(xPos, yPos)){}

void Player::moveLeft(){
	change_x += -speed_x;
	change_y = 0;
}

void Player::moveRight(){
	change_x += speed_x;
	change_y = 0;
}

void Player::moveUp(){
	change_x = 0;
	change_y -= speed_y;
}

void Player::moveDown(){
	change_x = 0;
	change_y += speed_y;
}

void Player::stop(){
	change_x = 0;
	change_y = 0;
}

void Player::update(float deltaTime){
	x += change_x * deltaTime;
	y += change_y * deltaTime;
	position.setX(x);
	position.setY(y);
	notifyObservers();
}

void Player::draw(){
	if(!rotate){
		int cell_width = 128;
		int current_frame = 9;
		float cell_division = 1.0 / 12.0;
		float s1 = current_frame * cell_division; // Calculate left horizontal texture coordinate
		float s2 = (current_frame * cell_division) + cell_division; // Calculate right horizontal texture coordinate
		float t1 = 0.0; // Calculate lower vertical texture coordinate
		float t2 = 1.0; // Calculate upper vertical texture coordinate
		glDrawSpriteSheet(image, int(x), int(y), cell_width, height, s1, s2, t1, t2);
	}
	else{
		rotateAngle += 90.0;
		glDrawSpriteRotate(image, int(x), int(y), width, height, rotateAngle);
	}
	if(rotateAngle > 360)
		rotateAngle = 0;
}

// Notify all observers of Player info
void Player::notifyObservers(){
	for(unsigned int i = 0; i < myObservers.size(); i++){
		myObservers.at(i)->notify(this);
	}
}

// Add observers to Player's list of observers
void Player::registerObserver(Observer *observer){
	myObservers.push_back(observer);
}

// Remove observer from Player's list of observers
void Player::unregisterObserver(Observer *observer){
	for(unsigned int i = 0; i < myObservers.size(); i++){
		if(myObservers.at(i)==observer){
			myObservers.erase(myObservers.begin()+i);
			return;
		}
	}
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