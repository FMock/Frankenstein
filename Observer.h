#ifndef OBSERVER_H
#define OBSERVER_H

class AnimatedSprite;

class Observer{
public:
	inline Observer(void){}
	template<typename T> void notify(T*);
	inline ~Observer(void){}
	virtual void respondToObserved(AnimatedSprite*)=0;
};

/*Observer receives notification from the observed*/
template<typename T> inline void Observer::notify(T* type){

	respondToObserved(type);
}

#endif