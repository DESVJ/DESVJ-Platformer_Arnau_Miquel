#include"eCoin.h"
#include "EnemyStateMachine.h"
#include "EntityManager.h"
#include "j1Player.h"


// Called before render is available
bool eCoins::Awake(pugi::xml_node& config)
{
	eEnemy::Awake(config);
	return true;
}


// Called each loop iteration
bool eCoins::Update(float dt)
{

	//Render Enemy
	App->render->Blit(spritesheet, collider->collider_rect.x, collider->collider_rect.y);

	return true;
}



bool eCoins::Save(pugi::xml_node& data) const {

	//Save all vars to XML file
	data.append_child("pickup");
	pugi::xml_node pickup_node = data.child("pickup").append_child("potion");
	pickup_node.append_child("position");
	pickup_node.child("position").append_attribute("x") = collider->collider_rect.x;
	pickup_node.child("position").append_attribute("y") = collider->collider_rect.y;
	pickup_node.child("position").append_attribute("w") = position_rect.w;
	pickup_node.child("position").append_attribute("h") = position_rect.h;

	pickup_node.append_attribute("flip") = flip;
	pickup_node.append_attribute("alive") = alive;
	pickup_node.append_attribute("respawn") = respawn;



	return true;
}


bool eCoins::Load(pugi::xml_node& data) {

	//Load all vars to xml file
	pugi::xml_node snake_node = data.child("pickup").child("potion");
	collider->collider_rect.x = snake_node.child("position").attribute("x").as_int();
	collider->collider_rect.y = snake_node.child("position").attribute("y").as_int();
	position_rect.w = snake_node.child("position").attribute("w").as_int();
	position_rect.h = snake_node.child("position").attribute("h").as_int();

	flip = snake_node.attribute("flip").as_bool();
	alive = snake_node.attribute("alive").as_bool();
	respawn = snake_node.attribute("respawn").as_bool();


	return true;
}