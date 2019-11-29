#include"eBatEnemy.h"
#include "EntityManager.h"
#include "j1Input.h"


// Called before render is available
bool eBatEnemy::Awake(pugi::xml_node& config)
{
	eEnemy::Awake(config.child("bat"));
	return true;
}


bool eBatEnemy::Start()
{


	return true;
}

// Called each loop iteration
bool eBatEnemy::Update(float dt)
{
	SDL_Rect current_frame = idle.GetCurrentFrame();

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

	App->colliders->MoveObject(&collider->collider_rect, { (int)round(speed.x), 0 }, this);
	App->colliders->MoveObject(&collider->collider_rect, { 0, (int)round(speed.y) }, this);

	//Calculate animation offset
	int animation_created_mov = 0;
	if (position_rect.w != 0)
	{
		animation_created_mov = collider->collider_rect.w - current_frame.w;
	}

	//Update player rect
	position_rect.w = current_frame.w;
	position_rect.h = -current_frame.h;

	position_rect.x = collider->collider_rect.x + (animation_created_mov / 2);
	position_rect.y = collider->collider_rect.y;

	//Render Enemy
	App->render->Blit(spritesheet, position_rect.x, position_rect.y - current_frame.h, &current_frame, flip);
	return true;
}
