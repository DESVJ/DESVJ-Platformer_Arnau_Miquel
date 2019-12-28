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
	TANG,
	PLAYER,
	ENEMY,
	PICKUP
};

//Collider info
struct Collider 
{
	SDL_Rect collider_rect;
	bool enabled;
	bool canBeJumped = false;
	Collider_Types collider_type;
};

class eCreature;

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

	bool typeColDetected;

	//Function to move objects by collision prediction
	void MoveObject(SDL_Rect*, p2Point<int>, eCreature* s_mod);

	//Load colliders
	void LoadColliders();

	//Check for rect collision
	bool CheckCollision(const SDL_Rect& rect, const SDL_Rect& r);
	bool CheckAbsoluteCollision(const SDL_Rect& rect, const SDL_Rect& r);

	void DrawColliders();
	void ClearColliders();
};

#endif // __j1COLLISION_H__