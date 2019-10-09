#ifndef __j1COLLISION_H__
#define __j1COLLISION_H__

#include "j1Module.h"
#include"p2Point.h"
#include"j1Map.h"
#include"p2Log.h"
#include"p2SString.h"
#include"p2List.h"

struct Collider 
{
	SDL_Rect collider_rect;
	p2SString collider_type;
};

struct Collider_Manager
{

	MapLayer* collider_layer = nullptr;
	p2List<Collider> collider_list;

	void MoveObject(p2Point<int>* currentPoint, p2Point<int> increment) 
	{
		LOG("Moving");
		//for itinerate all Colliders of the layer
		//look up collider type
		//currentpos + increment == colision?


	}

};


#endif // __j1COLLISION_H__