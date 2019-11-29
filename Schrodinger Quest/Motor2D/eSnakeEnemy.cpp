#include"eSnakeEnemy.h"
#include "EntityManager.h"
#include "j1Input.h"


// Called before render is available
bool eSnakeEnemy::Awake(pugi::xml_node& config)
{
	eEnemy::Awake(config.child("snake"));
	return true;
}

// Called each loop iteration
bool eSnakeEnemy::Update(float dt)
{
	
	if (App->entity_manager->Player->collider->collider_rect.y >= collider->collider_rect.y) 
	{
		if (PathFinding(App->entity_manager->Player->collider->collider_rect) == 0)
		{
			const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
			if (isGrounded)
			{
				const iPoint* origin = path->At(0);
				const iPoint* obj = path->At(1);
				if (obj != NULL)
				{
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
			speed.y = 2;
		}

	}
	else
	{
		speed.x = 0;
		speed.y = 2;
	}
	
	MoveAndDraw();

	return true;
}

