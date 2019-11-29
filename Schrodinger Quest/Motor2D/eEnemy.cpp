#include"eEnemy.h"


bool eEnemy::Awake(pugi::xml_node& config) 
{

	pugi::xml_document	player_info_file;
	player_info_file.load_file(config.child("load_file").child_value());
	pugi::xml_node player_node = player_info_file.child("map");

	CreateCollider(Collider_Types::ENEMY);

	LoadAnimationFromTMX(&player_node, &idle, "idle");
	LoadAnimationFromTMX(&player_node, &move, "move");
	LoadAnimationFromTMX(&player_node, &death, "death");

	detection_range = config.child("detection_range").attribute("value").as_int();

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

void eEnemy::MoveAndDraw() 
{
	SDL_Rect current_frame = idle.GetCurrentFrame();

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
}

bool eEnemy::CleanUp()
{
	eCreature::CleanUp();
	return true;
}

p2Point<bool> eEnemy::OnCollision(Collider* in_collider, SDL_Rect prediction, SDL_Rect* block, Direction dir, p2Point<bool> prev_res)
{

	//Allow the object to ignore down collisions (player jumping in top of platform)
	if (allowClippingCollider != nullptr && collider->collider_rect.y <= allowClippingCollider->collider_rect.y)
	{
		allowClippingCollider = nullptr;
	}


	if (in_collider != allowClippingCollider && in_collider->collider_type != Collider_Types::ENEMY)
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

					if (in_collider->canBeJumped)
					{
						allowClippingCollider = in_collider;
					}
					else
					{
						collider->collider_rect.y = block->y + block->h - prediction.h;
					}
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