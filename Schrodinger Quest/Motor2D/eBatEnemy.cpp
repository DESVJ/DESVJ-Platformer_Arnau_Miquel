#include"eBatEnemy.h"
#include "EntityManager.h"
#include "j1Input.h"


// Called before render is available
bool eBatEnemy::Awake(pugi::xml_node& config)
{

	//pugi::xml_document	player_info_file;
	////p2SString s = config.child("ground_enemy").child("snake").child("load_file").child_value();
	//player_info_file.load_file(config.child("ground_enemy").child("snake").child("load_file").child_value());
	//pugi::xml_node player_node = player_info_file.child("map");

	//LoadAnimationFromTMX(&player_node, &idle, "idle");

	//detection_range = config.child()


	LoadAnimationFromTMX(&entity_node, &idle, "idle");
	LoadAnimationFromTMX(&entity_node, &move, "move");
	LoadAnimationFromTMX(&entity_node, &death, "death");

	collision_rect.w = idle.GetCurrentFrameWithoutAnim().w;
	collision_rect.h = -idle.GetCurrentFrameWithoutAnim().h;

	if (!spritesheet)
	{
		//Remove hardcode when tmx of enemy is created
		spritesheet = App->tex->Load("textures/Enemy_Sprites/Bat_Sprite_Sheet.png");
	}

	LOG("%i, %i", collision_rect.w, collision_rect.h);
	return true;
}


bool eBatEnemy::Start()
{

	//TODO THIS IS HARDCODED, CANT USE IT ON AWAKE BECASUSE ITS CALLED DURING RUNTIME, NEEDS TO BE FIXED
	/*pugi::xml_document	player_info_file;
	player_info_file.load_file("textures/Enemy_Sprites/bat.tmx");
	pugi::xml_node player_node = player_info_file.child("map");
	*/
	//LoadAnimationFromTMX(&entity_node, &idle, "idle");
	//LoadAnimationFromTMX(&entity_node, &move, "move");
	//LoadAnimationFromTMX(&entity_node, &death, "death");

	//collision_rect.w = idle.GetCurrentFrameWithoutAnim().w;
	//collision_rect.h = -idle.GetCurrentFrameWithoutAnim().h;

	//if (!spritesheet)
	//{
	//	//Remove hardcode when tmx of enemy is created
	//	spritesheet = App->tex->Load("textures/Enemy_Sprites/Bat_Sprite_Sheet.png");
	//}

	//LOG("%i, %i", collision_rect.w, collision_rect.h);
	return true;
}

// Called each loop iteration
bool eBatEnemy::Update(float dt)
{
	SDL_Rect current_frame = idle.GetCurrentFrame();

	if (PathFinding(App->entity_manager->Player->collision_rect) == 0)
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

	App->colliders->MoveObject(&collision_rect, { (int)round(speed.x), 0 }, this);
	App->colliders->MoveObject(&collision_rect, { 0, (int)round(speed.y) }, this);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		int x = path->At(i)->x;
		int y = path->At(i)->y;
		App->map->Translate_Coord(&x, &y);
		iPoint pos = { x, y };
		App->render->DrawQuad({ pos.x, pos.y, 16, 16 }, 0, 255, 0, 50);
	}

	//Calculate animation offset
	int animation_created_mov = 0;
	if (position_rect.w != 0)
	{
		animation_created_mov = collision_rect.w - current_frame.w;
	}

	//Update player rect
	position_rect.w = current_frame.w;
	position_rect.h = -current_frame.h;

	position_rect.x = collision_rect.x + (animation_created_mov / 2);
	position_rect.y = collision_rect.y;

	//Render Enemy
	App->render->Blit(spritesheet, position_rect.x, position_rect.y - current_frame.h, &current_frame, flip);

	if (App->input->is_Debug_Mode)
	{
		App->render->DrawQuad(collision_rect, 255, 0, 0, 80);
	}
	return true;
}

bool eBatEnemy::CleanUp()
{

	eCreature::CleanUp();



	return true;
}


p2Point<bool> eBatEnemy::OnCollision(Collider* in_collider, SDL_Rect prediction, SDL_Rect* block, Direction dir, p2Point<bool> prev_res)
{

	//Allow the object to ignore down collisions (player jumping in top of platform)
	if (allowClippingCollider != nullptr && collision_rect.y <= allowClippingCollider->collider_rect.y)
	{
		allowClippingCollider = nullptr;
	}


	if (in_collider != allowClippingCollider)
	{
		//Is the collision inside x and x + w?
		if (prediction.x + prediction.w > block->x && prediction.x < block->x + block->w)
		{
			//Correct movement or move object in a normal way
			if (prediction.y >= block->y && prediction.y <= block->y + (block->h / 5) - prediction.h)
			{
				prev_res.y = true;
				if (dir == DOWN)
				{
					collision_rect.y = block->y;
				}
			}
			else if (prediction.y + prediction.h < block->y + block->h && prediction.y > block->y + (block->h / 2))
			{
				if (dir == UP)
				{
					prev_res.y = true;

					if (in_collider->canBeJumped)
					{
						allowClippingCollider = in_collider;
					}
					else
					{
						collision_rect.y = block->y + block->h - prediction.h;
					}
				}

			}
		}
		if (prediction.y > block->y && prediction.y + prediction.h < block->y + block->h)
		{
			prev_res.x = true;
			////Coliding with the sides of an object
			if (prediction.x <= block->x + block->w)
			{
				if (dir == LEFT)
				{
					collision_rect.x = block->x + block->w;
				}
			}
			else if (prediction.x + prediction.w >= block->x)
			{
				if (dir == RIGHT)
				{
					collision_rect.x = block->x - collision_rect.w;
				}
			}

		}
	}

	return prev_res;
}

void eBatEnemy::AfterCollision(p2Point<bool> col_result, SDL_Rect prediction, p2Point<int> increment)
{
	//If no movement correction is needed, therefore there is no collisions, just move the object to the predicted point
	if (col_result.x == false)
	{
		collision_rect.x = prediction.x;
	}
	if (col_result.y == false)
	{
		collision_rect.y = prediction.y;
	}
}

