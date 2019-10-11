#include"j1App.h"
#include"Collision_Manager.h"
#include"j1Window.h"


Collider_Manager::Collider_Manager() {

}
Collider_Manager::~Collider_Manager() {

}

void Collider_Manager::LoadColliders() 
{
	for (int i = 0; i < collider_layer->height; i++)
	{

		for (int j = 0; j < collider_layer->width; j++)
		{
			int n = collider_layer->Get(j, i);
			int gid = collider_layer->gid[n];
			if (gid != 0)
			{
				int x = j;
				int y = i;
				App->map->Translate_Coord(&x, &y);

				Collider newCollider;

				newCollider.collider_rect = {x, y, App->map->data.tile_width , App->map->data.tile_height };
				//Load collide type
				newCollider.enabled = true;

				collider_list.add(newCollider);

				
			}
		}
	}
			LOG("%i", collider_list.count());

}

void Collider_Manager::MoveObject(SDL_Rect* currentPoint, p2Point<int> increment) 
{

	SDL_Rect prediction = *currentPoint;
	prediction.x += increment.x;
	prediction.y += increment.y;

	Direction dir = DOWN;
	if (increment.x > 0)
	{
		dir = RIGHT;
	}
	else if (increment.x < 0)
	{
		dir = LEFT;
	}
	if (increment.y > 0)
	{
		dir = DOWN;
	}
	else if (increment.y < 0)
	{
		dir = UP;
	}


	bool colisionDetectedX = false;
	bool colisionDetectedY = false;

	for (int i = 0; i < collider_list.count(); i++)
	{
		SDL_Rect *block = &collider_list[i].collider_rect;

		if ((block->x + block->w) * App->win->GetScale() >= -App->render->camera.x && block->x * App->win->GetScale() <= -App->render->camera.x + App->win->width
			&& (block->y + block->h) * App->win->GetScale() >= -App->render->camera.y && block->y * App->win->GetScale() <= -App->render->camera.y + App->win->height)
		{
			if (CheckCollision(prediction, *block))
			{
				//LOG("aa");
				//Maybe going too fast can cause clipping
				if (prediction.x + prediction.w > block->x && prediction.x < block->x + block->w)
				{
					colisionDetectedY = true;

					if (prediction.y >= block->y && prediction.y <= block->y + (block->h / 2))
					{
						if (dir == DOWN)
						{
							currentPoint->y = block->y;
						}
					}
					else if (prediction.y + prediction.h < block->y + block->h && prediction.y > block->y + (block->h / 2))
					{
						if (dir == UP)
						{
							currentPoint->y = block->y + block->h - currentPoint->h;
						}

					}
				}
				if (prediction.y > block->y && prediction.y + prediction.h < block->y + block->h)
				{
					colisionDetectedX = true;
					////Coliding with the sides of an object
					if (prediction.x > block->x + block->w)
					{
						if (dir == LEFT)
						{
							currentPoint->x = block->x + block->w;
						}
					}
					else if (prediction.x + prediction.w < block->x)
					{
						if (dir == RIGHT)
						{
							currentPoint->x = block->x - currentPoint->w;
						}
					}

				}
			}

			//LOG("%i", dir);
		}
	}



	if (!colisionDetectedX)
	{
		currentPoint->x = prediction.x;
	}
	if (!colisionDetectedY)
	{
		currentPoint->y = prediction.y;
	}
}