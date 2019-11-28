#ifndef _ECREATURE_H
#define _ECREATURE_H
#include "Entity.h"
#include "p2Point.h"
#include "Collision_Manager.h"
#include "Animation.h"
#include "p2SString.h"
#include "j1Textures.h"
#include "j1Pathfinding.h"

struct Collider;
enum Direction;
class eCreature : public Entity
{
public:
	eCreature(Types type) :Entity(type) {

	}

	// Destructor
	virtual ~eCreature() {

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
	virtual bool CleanUp();

	//Load animation pushbacks by name
	virtual pugi::xml_node LoadAnimationFromTMX(pugi::xml_node* animation_node, Animation* anim, const char* name);

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

	int PathFinding(SDL_Rect objective);

	virtual p2Point<bool> OnCollision(Collider*, SDL_Rect, SDL_Rect*, Direction, p2Point<bool>)
	{
		return { false, false };
	}
	virtual void AfterCollision(p2Point<bool>, SDL_Rect, p2Point<int>) {}

public:
	p2Point<float> speed;
	p2Point<int> spawn;

	bool flip;
	bool alive;
	bool respawn;

	//General animations
	Animation idle;
	Animation death;
	Animation move;

	//General collisions
	bool typeColDetected;
	Collider* allowClippingCollider;

	//General pathfinding
	int detection_range;

	//General gfx
	SDL_Texture* spritesheet;
	int PathNumber;

};

#endif
