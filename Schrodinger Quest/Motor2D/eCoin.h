#ifndef _ECOIN_H_
#define _ECOIN_H_

#include"p2Point.h"
#include"p2Log.h"
#include"p2SString.h"
#include"p2List.h"
#include"eCreature.h"
#include"eEnemy.h"


class eEnemy;
//Snake class
class eCoins : public eEnemy
{

public:

	eCoins(Types type, pugi::xml_node snake_node) :eEnemy(type) {
		name.create("pickups");
		entity_type = type;
		entity_node = snake_node;
	}

	// Destructor
	~eCoins() {

	}

	// Called before render is available
	bool Awake(pugi::xml_node&);

	//// Called each loop iteration
	bool Update(float dt);

	//Save
	bool Save(pugi::xml_node&) const;

	//Load
	bool Load(pugi::xml_node&);
};
#endif // __ESNAKEENEMY_H__

