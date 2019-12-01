#include"eEnemy.h"


bool eEnemy::Awake(pugi::xml_node& config) 
{
	alive = true;
	pugi::xml_document	enemy_info_file;
	enemy_info_file.load_file(config.child("load_file").child_value());
	pugi::xml_node enemy_node = enemy_info_file.child("map");

	CreateCollider(Collider_Types::ENEMY);

	LoadAnimationFromTMX(&enemy_node, &idle, "idle");
	LoadAnimationFromTMX(&enemy_node, &move, "move");
	LoadAnimationFromTMX(&enemy_node, &death, "death");

	detection_range = config.child("detection_range").attribute("value").as_int();
	en_state = (Enemy_State)config.child("en_state").attribute("value").as_int();

	collider->collider_rect.w = idle.GetCurrentFrameWithoutAnim().w;
	collider->collider_rect.h = -idle.GetCurrentFrameWithoutAnim().h;

	if (!spritesheet)
	{
		//Remove hardcode when tmx of enemy is created
		spritesheet = App->tex->Load(config.child("texture_source").child_value());
	}

	LOG("%i, %i", collider->collider_rect.w, collider->collider_rect.h);
	return true;
}

void eEnemy::MoveAndDraw(float dt, SDL_Rect current_frame) 
{

	App->colliders->MoveObject(&collider->collider_rect, { (int)round(speed.x*60*dt), 0 }, this);
	if (speed.y==1)	App->colliders->MoveObject(&collider->collider_rect, { 0, (int)round(speed.y) }, this);
	else App->colliders->MoveObject(&collider->collider_rect, { 0, (int)round(speed.y*60*dt) }, this);

	//Calculate animation offset
	int animation_created_mov = 0;
	if (position_rect.w != 0)
	{
		animation_created_mov = collider->collider_rect.w - current_frame.w;
	}

	//Update player rect
	if (alive) 
	{
		position_rect.w = current_frame.w;
		position_rect.h = -current_frame.h;

		position_rect.x = collider->collider_rect.x + (animation_created_mov / 2);
		position_rect.y = collider->collider_rect.y;
	}

	//Render Enemy
	App->render->Blit(spritesheet, position_rect.x, position_rect.y - current_frame.h, &current_frame, flip);
}

void eEnemy::DoPathFinding(p2Point<float> speedIn, p2Point<float> standarSpeed)
{
	if (en_state == Enemy_State::chase)
	{
		PathFinding(App->entity_manager->Player->collider->collider_rect);

		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
		const iPoint* origin = path->At(0);
		const iPoint* obj = path->At(1);
		if (obj != NULL)
		{
			speed.x = standarSpeed.x;
			speed.y = standarSpeed.y;
			if (obj->x < origin->x)
			{
				speed.x = -speedIn.x;
				flip = SDL_FLIP_HORIZONTAL;
			}
			if (obj->x > origin->x)
			{
				speed.x = speedIn.x;
				flip = SDL_FLIP_NONE;
			}


			if (obj->y < origin->y)
			{
				speed.y = -speedIn.y;
			}
			if (obj->y > origin->y)
			{
				speed.y = speedIn.y;
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
	else if (en_state == Enemy_State::move)
	{
		speed.x = standarSpeed.x;
		speed.y = standarSpeed.y;
	}
	else
	{
		speed.x = standarSpeed.x;
		speed.y = standarSpeed.y;
	}

}

bool eEnemy::CleanUp()
{
	eCreature::CleanUp();
	return true;
}

p2Point<bool> eEnemy::OnCollision(Collider* in_collider, SDL_Rect prediction, SDL_Rect* block, Direction dir, p2Point<bool> prev_res)
{


	if (in_collider->collider_type != Collider_Types::ENEMY)
	{
		//Is the collision inside x and x + w?
		if (prediction.x + prediction.w > block->x&& prediction.x < block->x + block->w)
		{
			//Correct movement or move object in a normal way
			if (prediction.y >= block->y && prediction.y <= block->y + (block->h / 5) - prediction.h)
			{
				prev_res.y = true;
				if (dir == DOWN)
				{
					collider->collider_rect.y = block->y;
					isGrounded = true;
				}
			}
			else if (prediction.y + prediction.h < block->y + block->h && prediction.y >block->y + (block->h / 2))
			{
				if (dir == UP)
				{
					prev_res.y = true;
					collider->collider_rect.y = block->y + block->h - prediction.h;
				}

			}
		}
		if (prediction.y > block->y&& prediction.y + prediction.h < block->y + block->h)
		{
			prev_res.x = true;
			////Coliding with the sides of an object
			if (prediction.x <= block->x + block->w)
			{
				if (dir == LEFT)
				{
					collider->collider_rect.x = block->x + block->w;
				}
			}
			else if (prediction.x + prediction.w >= block->x)
			{
				if (dir == RIGHT)
				{
					collider->collider_rect.x = block->x - collider->collider_rect.w;
				}
			}

		}
	}

	if (in_collider->collider_type == Collider_Types::PLAYER) {
		LOG("BBB");
	}

	return prev_res;
}

void eEnemy::AfterCollision(p2Point<bool> col_result, SDL_Rect prediction, p2Point<int> increment)
{
	//If no movement correction is needed, therefore there is no collisions, just move the object to the predicted point
	if (col_result.x == false)
	{
		collider->collider_rect.x = prediction.x;
	}
	if (col_result.y == false)
	{
		collider->collider_rect.y = prediction.y;
		isGrounded = false;
	}
}