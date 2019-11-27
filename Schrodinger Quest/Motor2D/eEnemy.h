#ifndef __EENEMY_H__
#define __EENEMY_H__

#include"p2Point.h"
#include"p2Log.h"
#include"p2SString.h"
#include"p2List.h"
#include"eCreature.h"


//Player class
class eEnemy : public eCreature
{

public:

	eEnemy(Types type) :eCreature(type) {
		name.create("enemy");
	}

	// Destructor
	virtual ~eEnemy() {

	}

	bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start() {
		return true;
	}

	//// Called each loop iteration
	//bool PreUpdate();
	bool Update(float dt) {
		return true;
	}

	// Called before quitting
	virtual bool CleanUp() {
		return true;
	}

};
#endif // __EENEMY_H__
