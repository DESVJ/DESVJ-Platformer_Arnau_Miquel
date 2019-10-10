#ifndef __j1COLLISION_H__
#define __j1COLLISION_H__

#include "j1Module.h"
#include"p2Point.h"
#include"j1Map.h"
#include"p2Log.h"
#include"p2SString.h"
#include"p2List.h"
#include"j1Render.h"
#include"j1App.h"

struct Collider 
{
	SDL_Rect collider_rect = {0, 0, 0, 0};
	p2SString collider_type;
};

enum Direction {
	UP, DOWN, RIGHT, LEFT
};

struct Collider_Manager
{

	MapLayer* collider_layer = nullptr;

	void MoveObject(SDL_Rect* currentPoint, p2Point<int> increment) 
	{
		//currentPoint->y += increment.y;
		//for itinerate all Colliders of the layer
		//look up collider type
		//currentpos + increment == colision?

		SDL_Rect prediction = *currentPoint;
		prediction.x += increment.x;
		prediction.y += increment.y;

		Direction dir;

		if (increment.x > 0) 
		{
			dir = RIGHT;
		}
		else if(increment.x < 0)
		{
			dir = LEFT;
		}
		if (increment.y > 0) 
		{
			dir = DOWN;
		}
		else if(increment.y < 0)
		{
			dir = UP;
		}



		j1Map::Test(prediction, *currentPoint, dir);


		
	}

	bool CheckCollision(const SDL_Rect& rect, const SDL_Rect& r)
	{
		bool detectedX = true;
		bool detectedY = true;
		// TODO 0: Return true if there is an overlap
		// between argument "r" and property "rect"

		if ((rect.x + rect.w) < r.x || (r.x + r.w) < rect.x) 
		{
			detectedX = false;
		}

		if (rect.y < r.y || (rect.y + rect.h) > r.y + r.h)
		{
			detectedY = false;
		}

		return detectedX && detectedY;
	}

};


#endif // __j1COLLISION_H__