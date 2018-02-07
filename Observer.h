#pragma once
#ifndef OBSERVER_H
#define OBSERVER_H

class Player;

/* Observer is a pure virtual class that other game classes can implement
 * so that they can be notified of player information.
 */
class Observer{
public:
	Observer(void);
	virtual void notify(Player* myPlayer)=0;
	~Observer(void);
};

#endif