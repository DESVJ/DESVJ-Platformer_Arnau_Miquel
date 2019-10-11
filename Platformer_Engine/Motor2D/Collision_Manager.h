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


enum Collider_Types
{
	WALKEABLE, KILL, CLIMB
};

struct Collider 
{
	SDL_Rect collider_rect = {0, 0, 0, 0};
	bool enabled = true;
	Collider_Types collider_type;
};

enum Direction {
	UP, DOWN, RIGHT, LEFT
};

class Collider_Manager
{

public:
	Collider_Manager();
	~Collider_Manager();

	p2List<Collider> collider_list;

	void MoveObject(SDL_Rect*, p2Point<int>);

	void LoadColliders();

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