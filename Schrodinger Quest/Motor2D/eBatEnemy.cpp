#include"eBatEnemy.h"
#include "EntityManager.h"
#include "j1Input.h"


// Called before render is available
bool eBatEnemy::Awake(pugi::xml_node& config)
{
	eEnemy::Awake(config.child("bat"));
	return true;
}


// Called each loop iteration
bool eBatEnemy::Update(float dt)
{
	SDL_Rect current_frame = idle.GetCurrentFrame(dt);

	if (PathFinding(App->entity_manager->Player->collider->collider_rect) == 0)
	{

		//App->colliders->MoveObject(&position_rect, {0, -5}, this);

		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
		const iPoint* origin = path->At(0);
		const iPoint* obj = path->At(1);
		if (obj != NULL)
		{
			speed.x = 0;
			speed.y = 0;
			if (obj->x < origin->x)
			{
				speed.x = -1;
				flip = SDL_FLIP_HORIZONTAL;
			}
			if (obj->x > origin->x)
			{
				speed.x = 1;
				flip = SDL_FLIP_NONE;
			}


			if (obj->y < origin->y)
			{
				speed.y = -2;
			}
			if (obj->y > origin->y)
			{
				speed.y = 2;
			}


		}
		if (App->input->is_Debug_Mode)
		{
			for (uint i = 0; i < path->Count(); ++i)
			{
				int x = path->At(i)->x;
				int y = path->At(i)->y;
				App->map->Translate_Coord(&x, &y);
				iPoint pos = { x, y };
				App->render->DrawQuad({ pos.x, pos.y, 16, 16 }, 0, 255, 0, 50);
			}
		}

		App->pathfinding->ClearPath();

	}
	else
	{
		speed.x = 0;
		speed.y = 0;
	}

	MoveAndDraw(dt);

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
	bat_node.append_attribute("timer_idle") = timer_idle;
	
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
	respawn = bat_node.attribute("respawn").as_bool();
	not_chase_tang_mode = bat_node.attribute("not_chase_tang_mode").as_bool();
	timer_idle= bat_node.attribute("timer_idle").as_bool();
	en_state = (Enemy_State)bat_node.attribute("en_state").as_int();
	en_state_update = (Enemy_State)bat_node.attribute("en_state_update").as_int();

	return true;
}