#ifndef _CREATURE_H
#define _CREATURE_H
#include "Entity.h"
#include "p2Point.h"

class Creature : public Entity 
{
public:
	Creature(Types type) :Entity(type) {

	}

	// Destructor
	virtual ~Creature() {

	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	//Called when loading the game
	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	//Called when saving the game
	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

public:
	p2Point<float> speed;
	p2Point<int> spawn;
};

#endif
