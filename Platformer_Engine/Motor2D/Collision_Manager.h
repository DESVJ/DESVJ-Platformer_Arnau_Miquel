#ifndef __j1COLLISION_H__
#define __j1COLLISION_H__

#include "j1Module.h"
#include"p2Point.h"
#include"j1Map.h"
#include"p2Log.h"

struct Collider 
{
	SDL_Rect collider_rect;
};

struct Collider_Manager
{

	MapLayer* collider_layer = nullptr;

	void MoveObject(p2Point<int>* currentPoint, p2Point<int> increment) 
	{
		LOG("Moving");
	}

};


#endif // __j1COLLISION_H__