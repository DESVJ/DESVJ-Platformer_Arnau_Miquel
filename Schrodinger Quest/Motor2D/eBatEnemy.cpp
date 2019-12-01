#include"eBatEnemy.h"
#include "EntityManager.h"
#include "EnemyStateMachine.h"
#include "j1Input.h"


// Called before render is available
bool eBatEnemy::Awake(pugi::xml_node& config)
{
	eEnemy::Awake(config.child("bat"));
	return true;
}

bool eBatEnemy::PreUpdate()
{
	if (PathFinding(App->entity_manager->Player->collider->collider_rect) == 0)player_nearby = true;
	else player_nearby = false;


	CheckInputs(not_chase_tang_mode, alive, player_nearby, timer_idle, en_state, en_state_update);

	return true;
}


// Called each loop iteration
bool eBatEnemy::Update(float dt)
{
	Animation* current_animation;
	bool reset_animation = CheckState(timer_idle, en_state, en_state_update);
	current_animation = ExecuteState(speed, flip, alive, timer_idle, en_state, &idle, &move, &death);
	if (current_animation == &move)current_animation = &idle;

	if (reset_animation == true)
	{
		current_animation->Reset();
	}

	SDL_Rect current_frame = current_animation->GetCurrentFrame(dt);

	if (alive == false)
	{
		if (current_animation->GetFinished() == 1)
		{
			alive = true;
		}
	}
	else
	{
		DoPathFinding({ 1, 2 }, {0, 0});
	}

	MoveAndDraw(dt, current_frame);
	
	return true;
}


bool eBatEnemy::Save(pugi::xml_node& data) const {

	//Save all vars to XML file
	data.append_child("fly_enemy");
	pugi::xml_node bat_node = data.child("fly_enemy").append_child("bat");
	bat_node.append_child("position");
	bat_node.child("position").append_attribute("x") = collider->collider_rect.x;
	bat_node.child("position").append_attribute("y") = collider->collider_rect.y;
	bat_node.child("position").append_attribute("w") = position_rect.w;
	bat_node.child("position").append_attribute("h") = position_rect.h;
	bat_node.append_child("speed");
	bat_node.child("speed").append_attribute("x") = speed.x;
	bat_node.child("speed").append_attribute("y") = speed.y;
	bat_node.append_attribute("flip") = flip;;
	bat_node.append_attribute("alive") = alive;
	bat_node.append_attribute("player_nearby") = player_nearby;
	bat_node.append_attribute("respawn") = respawn;
	switch (en_state)
	{
	case Enemy_State::chase:
		bat_node.append_attribute("en_state") = 3;
		break;
	case Enemy_State::dead:
		bat_node.append_attribute("en_state") = 4;
		break;
	case Enemy_State::idle:
		bat_node.append_attribute("en_state") = 1;
		break;
	case Enemy_State::move:
		bat_node.append_attribute("en_state") = 2;
		break;
	}
	switch (en_state_update)
	{
	case Enemy_State::chase:
		bat_node.append_attribute("en_state_update") = 3;
		break;
	case Enemy_State::dead:
		bat_node.append_attribute("en_state_update") = 4;
		break;
	case Enemy_State::idle:
		bat_node.append_attribute("en_state_update") = 1;
		break;
	case Enemy_State::move:
		bat_node.append_attribute("en_state_update") = 2;
		break;
	}
	bat_node.append_attribute("not_chase_tang_mode") = not_chase_tang_mode;
	bat_node.append_attribute("timer_idle") = timer_idle.Get_Start();
	
	return true;
}


bool eBatEnemy::Load(pugi::xml_node& data) {

	//Load all vars to xml file
	pugi::xml_node bat_node = data.child("fly_enemy").child("bat");
	collider->collider_rect.x = bat_node.child("position").attribute("x").as_int();
	collider->collider_rect.y = bat_node.child("position").attribute("y").as_int();
	position_rect.w = bat_node.child("position").attribute("w").as_int();
	position_rect.h = bat_node.child("position").attribute("h").as_int();
	speed.x = bat_node.child("speed").attribute("x").as_float();
	speed.y = bat_node.child("speed").attribute("y").as_float();
	flip = bat_node.attribute("flip").as_bool();
	alive = bat_node.attribute("alive").as_bool();
	player_nearby = bat_node.attribute("player_nearby").as_bool();
	respawn = bat_node.attribute("respawn").as_bool();
	not_chase_tang_mode = bat_node.attribute("not_chase_tang_mode").as_bool();
	timer_idle.Set_Start(bat_node.attribute("timer_idle").as_bool());
	en_state = (Enemy_State)bat_node.attribute("en_state").as_int();
	en_state_update = (Enemy_State)bat_node.attribute("en_state_update").as_int();

	return true;
}