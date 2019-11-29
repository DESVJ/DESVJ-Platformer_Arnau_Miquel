#ifndef __EBATENEMY_H__
#define __EBATENEMY_H__

#include"p2Point.h"
#include"p2Log.h"
#include"p2SString.h"
#include"p2List.h"
#include"eCreature.h"
#include"eGroundEnemy.h"


class eEnemy;
//Player class
class eBatEnemy : public eEnemy
{

public:

	eBatEnemy(Types type, pugi::xml_node bat_node) :eEnemy(type) {
		name.create("enemy_info");
		entity_type = type;
		entity_node=bat_node;
	}

	// Destructor
	~eBatEnemy() {

	}

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	//// Called each loop iteration
	//bool PreUpdate();
	bool Update(float dt);

	////Start level again
	//void Start_F3();

	////Save
	//bool Save(pugi::xml_node&) const;

	////Load
	//bool Load(pugi::xml_node&);

	////Load animation pushbacks by name
	//void LoadAnimationFromTMX(pugi::xml_node* animation_node, Animation* anim, const char* name);

	////Load sound by TMX
	//void LoadSoundFXFromTMX(pugi::xml_node* sound_node, unsigned int& fx, const char* name);

	////Move player to spawn
	//void MoveToSpawn();
};
#endif // __EBATENEMY_H__


