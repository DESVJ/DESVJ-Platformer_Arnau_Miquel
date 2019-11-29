#include"eSnakeEnemy.h"
#include "EntityManager.h"
#include "j1Input.h"


// Called before render is available
bool eSnakeEnemy::Awake(pugi::xml_node& config)
{
	eEnemy::Awake(config.child("snake"));
	return true;
}

bool eSnakeEnemy::Start()
{

	//TODO THIS IS HARDCODED, CANT USE IT ON AWAKE BECASUSE ITS CALLED DURING RUNTIME, NEEDS TO BE FIXED
	//pugi::xml_document	player_info_file;
	//player_info_file.load_file("textures/Enemy_Sprites/snake.tmx");
	//pugi::xml_node player_node = player_info_file.child("map");

	//LoadAnimationFromTMX(&player_node, &idle, "idle");
	//LoadAnimationFromTMX(&player_node, &move, "move");
	//LoadAnimationFromTMX(&player_node, &death, "death");
	//
	//collision_rect.w = idle.GetCurrentFrameWithoutAnim().w;
	//collision_rect.h = -idle.GetCurrentFrameWithoutAnim().h;

	//if (!spritesheet)
	//{
	//	//Remove hardcode when tmx of enemy is created
	//	spritesheet = App->tex->Load("textures/Enemy_Sprites/Cobra_Sprite_Sheet.png");
	//}

	//LOG("%i, %i", collision_rect.w, collision_rect.h);
	return true;
}

// Called each loop iteration
bool eSnakeEnemy::Update(float dt)
{
	SDL_Rect current_frame = idle.GetCurrentFrame();

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
	
	App->colliders->MoveObject(&collider->collider_rect, { (int)round(speed.x), 0}, this);
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

