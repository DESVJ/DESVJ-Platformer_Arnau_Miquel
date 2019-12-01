#ifndef __ESNAKEENEMY_H__
#define __ESNAKEENEMY_H__

#include"p2Point.h"
#include"p2Log.h"
#include"p2SString.h"
#include"p2List.h"
#include"eCreature.h"
#include"eEnemy.h"


class eEnemy;
//Snake class
class eSnakeEnemy : public eEnemy
{

public:

	eSnakeEnemy(Types type, pugi::xml_node snake_node) :eEnemy(type) {
		name.create("enemy_info");
		entity_type = type;
		entity_node = snake_node;
	}

	// Destructor
	~eSnakeEnemy() {

	}

	// Called before render is available
	bool Awake(pugi::xml_node&);

	//// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);

	//Save
	bool Save(pugi::xml_node&) const;

	//Load
	bool Load(pugi::xml_node&);
};
#endif // __ESNAKEENEMY_H__

