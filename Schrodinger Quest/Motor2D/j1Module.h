// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"
#include "SDL/include/SDL.h"

class j1App;

class j1Module
{
public:

	j1Module() : active(false)
	{}

	//Called on module initialization
	void Init()
	{
		active = true;
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

	// Called each loop iteration
	virtual bool PostUpdate()
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

	virtual void OnCollision(/*Collider_Types type*/) {};

public:

	//Name of the module
	p2SString	name;

	//State of the module 
	bool		active;

};

#endif // __j1MODULE_H__