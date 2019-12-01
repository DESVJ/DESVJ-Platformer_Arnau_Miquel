#include"eSnakeEnemy.h"
#include "EnemyStateMachine.h"
#include "EntityManager.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Timer.h"


// Called before render is available
bool eSnakeEnemy::Awake(pugi::xml_node& config)
{
	eEnemy::Awake(config.child("snake"));
	return true;
}


bool eSnakeEnemy::PreUpdate() 
{
	if (App->entity_manager->Player->player.player_tang_mode == true)not_chase_tang_mode = true;
	else not_chase_tang_mode = false;
	if (PathFinding(App->entity_manager->Player->collider->collider_rect) == 0)player_nearby = true;
	else player_nearby = false;


	CheckInputs(not_chase_tang_mode, alive, player_nearby, timer_idle, en_state, en_state_update);

	return true;
}


// Called each loop iteration
bool eSnakeEnemy::Update(float dt)
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

	speed.x = 0;
	speed.y = 2;
	if (alive == false)
	{
		if (current_animation->GetFinished() == 1)
		{
			alive = true;
		}
	}
	else if (App->entity_manager->Player->collider->collider_rect.y >= collider->collider_rect.y)
	{
		DoPathFinding({ 1, 2 }, { 0, 1});
	}
	MoveAndDraw(dt, current_frame);

	return true;
}



bool eSnakeEnemy::Save(pugi::xml_node& data) const {

	//Save all vars to XML file
	data.append_child("ground_enemy");
	pugi::xml_node snake_node = data.child("ground_enemy").append_child("snake");
	snake_node.append_child("position");
	snake_node.child("position").append_attribute("x") = collider->collider_rect.x;
	snake_node.child("position").append_attribute("y") = collider->collider_rect.y;
	snake_node.child("position").append_attribute("w") = position_rect.w;
	snake_node.child("position").append_attribute("h") = position_rect.h;
	snake_node.append_child("speed");
	snake_node.child("speed").append_attribute("x") = speed.x;
	snake_node.child("speed").append_attribute("y") = speed.y;
	snake_node.append_attribute("flip") = flip;;
	snake_node.append_attribute("alive") = alive;
	snake_node.append_attribute("player_nearby") = player_nearby;
	snake_node.append_attribute("respawn") = respawn;
	switch (en_state)
	{
	case Enemy_State::chase:
		snake_node.append_attribute("en_state") = 3;
		break;
	case Enemy_State::dead:
		snake_node.append_attribute("en_state") = 4;
		break;
	case Enemy_State::idle:
		snake_node.append_attribute("en_state") = 1;
		break;
	case Enemy_State::move:
		snake_node.append_attribute("en_state") = 2;
		break;
	}
	switch (en_state_update)
	{
	case Enemy_State::chase:
		snake_node.append_attribute("en_state_update") = 3;
		break;
	case Enemy_State::dead:
		snake_node.append_attribute("en_state_update") = 4;
		break;
	case Enemy_State::idle:
		snake_node.append_attribute("en_state_update") = 1;
		break;
	case Enemy_State::move:
		snake_node.append_attribute("en_state_update") = 2;
		break;
	}
	snake_node.append_attribute("not_chase_tang_mode") = not_chase_tang_mode;
	snake_node.append_attribute("timer_idle") = timer_idle.Get_Start();

	return true;
}


bool eSnakeEnemy::Load(pugi::xml_node& data) {

	//Load all vars to xml file
	pugi::xml_node snake_node = data.child("ground_enemy").child("snake");
	collider->collider_rect.x = snake_node.child("position").attribute("x").as_int();
	collider->collider_rect.y = snake_node.child("position").attribute("y").as_int();
	position_rect.w = snake_node.child("position").attribute("w").as_int();
	position_rect.h = snake_node.child("position").attribute("h").as_int();
	speed.x = snake_node.child("speed").attribute("x").as_float();
	speed.y = snake_node.child("speed").attribute("y").as_float();
	flip = snake_node.attribute("flip").as_bool();
	alive = snake_node.attribute("alive").as_bool();
	player_nearby = snake_node.attribute("player_nearby").as_bool();
	respawn = snake_node.attribute("respawn").as_bool();
	not_chase_tang_mode = snake_node.attribute("not_chase_tang_mode").as_bool();
	timer_idle.Set_Start(snake_node.attribute("timer_idle").as_uint());
	en_state = (Enemy_State)snake_node.attribute("en_state").as_int();
	en_state_update = (Enemy_State)snake_node.attribute("en_state_update").as_int();

	return true;
}