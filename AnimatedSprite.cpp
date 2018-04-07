#include"AnimatedSprite.h"


AnimatedSprite::AnimatedSprite(){}

AnimatedSprite::AnimatedSprite(float xPos, float yPos, int w, int h, AnimationDef& animDef, const std::string& n) :  
							   speed_x(140.0), speed_y(140.0), animationDef(animDef),
							   facingDirection(facingDirections::RIGHT), name(n),
							   Sprite(xPos, yPos, w, h){}

AnimatedSprite::~AnimatedSprite(){}

/*Update position information, animationDef info, and notify any observers*/
void AnimatedSprite::update(float deltaTime){
	x += change_x * deltaTime;
	y += change_y * deltaTime;
	position.setX(x);
	box.setX(abs(x));
	position.setY(y);
	box.setY(abs(y));
	animationDef.update(deltaTime);
	notifyObservers();
}

/* Draw this AnimatedSprite to the screen */
void AnimatedSprite::draw(){

	glDrawFrame(animationDef.animations.at(animationDef.getCurrentAnimation()).image, 
			    int(x), int(y), animationDef.getFrameWidth(), animationDef.getFrameHeight(), 
				animationDef.animations.at(animationDef.getCurrentAnimation()).s1, 
				animationDef.animations.at(animationDef.getCurrentAnimation()).s2, 
				animationDef.animations.at(animationDef.getCurrentAnimation()).t1, 
				animationDef.animations.at(animationDef.getCurrentAnimation()).t2);
}

/* Change the currently playing animation to x
 * param x - the next animation to play*/
void AnimatedSprite::changeAnimation(int x){
	animationDef.previousAnimation = animationDef.getCurrentAnimation();
	animationDef.setCurrentAnimation(x);

	// adjust sprites facing direction to correspond with it's current animation facing direction
	facingDirection = animationDef.getFacingDirection();
}

void AnimatedSprite::moveLeft(){
	change_x += -speed_x;
	change_y = 0;
}

void AnimatedSprite::moveRight(){
	change_x += speed_x;
	change_y = 0;
}

void AnimatedSprite::moveUp(){
	change_x = 0;
	change_y -= speed_y;
}

void AnimatedSprite::moveDown(){
	change_x = 0;
	change_y += speed_y;
}

void AnimatedSprite::stop(){
	change_x = 0;
	change_y = 0;
}

int AnimatedSprite::getCurrentAnimation()const{
	return animationDef.getCurrentAnimation();
}

// Notify all observers of this sprites info
void AnimatedSprite::notifyObservers(){
	for(unsigned int i = 0; i < myObservers.size(); i++){
		myObservers.at(i)->notify(this);
	}
}

// Add observers to this sprites list of observers
void AnimatedSprite::registerObserver(Observer *observer){
	myObservers.push_back(observer);
}

// Remove an observer from this sprites list of observers
void AnimatedSprite::unregisterObserver(Observer *observer){
	for(unsigned int i = 0; i < myObservers.size(); i++){
		if(myObservers.at(i)==observer){
			myObservers.erase(myObservers.begin()+i);
			return;
		}
	}
}

int AnimatedSprite::getFacingDirection()const{
	return facingDirection;
}

void AnimatedSprite::setFacingDirection(int x){
	if(x > 0)
		facingDirection = x;
}

std::string AnimatedSprite::to_string()const{
	std::ostringstream oss;
	oss << "x = " << x << std::endl
		<< "y = " << y << std::endl
	    << "position x = " << position.getX() << std::endl
		<< "AABB x = " << box.x << "\n"
		<< "position y = " << position.getY() << std::endl
		<< "AABB y = " << box.y << "\n"
		<< "width = " << width << std::endl
		<< "AABB w = " << box.w << "\n"
		<< "height = " << height << std::endl
		<< "AABB h = " << box.h << "\n"
		<< "facing direction = " << facingDirection << std::endl
		<< "current animation = " << getCurrentAnimation() << std::endl;
	return oss.str();
}
