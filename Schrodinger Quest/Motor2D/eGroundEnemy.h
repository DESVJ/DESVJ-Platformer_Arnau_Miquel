#ifndef __EGROUNDENEMY_H__
#define __EGROUNDENEMY_H__

#include"p2Point.h"
#include"p2Log.h"
#include"p2SString.h"
#include"p2List.h"
#include"eCreature.h"


class eCreature;
//Player class
class eGroundEnemy : public eCreature
{

public:

	eGroundEnemy(Types type) :eCreature(type) {

	}
	
	// Destructor
	virtual ~eGroundEnemy() {

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
#endif // __EGROUNDENEMY_H__
