//Code by Miquel Suau && Arnau Falagueras

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

//Collide type
enum Collider_Types
{
	WALKEABLE, 
	KILL, 
	CLIMB, 
	TANG
};

//Collider info
struct Collider 
{
	SDL_Rect collider_rect;
	bool enabled;
	bool canBeJumped;
	Collider_Types collider_type;
};

//Movement direction
enum Direction 
{
	NONE_DIRECTION, 
	UP, 
	DOWN, 
	RIGHT, 
	LEFT
};

class Collider_Manager
{

public:
	Collider_Manager();
	~Collider_Manager();

	//List of colliders
	p2List<Collider> collider_list;

	Collider* allowClippingCollider;
	bool typeColDetected;

	//Function to move objects by collision prediction
	void MoveObject(SDL_Rect*, p2Point<int>, bool isPlayer = false);

	//Load colliders
	void LoadColliders();

	//Check for rect collision
	bool CheckCollision(const SDL_Rect& rect, const SDL_Rect& r)
	{
		bool detectedX = true;
		bool detectedY = true;

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

	bool CheckAbsoluteCollision(const SDL_Rect& rect, const SDL_Rect& r)
	{

		bool detectedX = false;
		bool detectedY = false;

		//Check if rect is full inside of r
		if (rect.x > r.x && rect.x + rect.w < r.x + r.w)
		{
			detectedX = true;
		}

		if (rect.y < r.y + r.h && rect.y + rect.h > r.y)
		{
			detectedY = true;
		}

		return detectedX && detectedY;

	}
};

#endif // __j1COLLISION_H__