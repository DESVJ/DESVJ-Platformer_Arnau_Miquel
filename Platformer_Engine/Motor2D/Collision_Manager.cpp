//Code by Miquel Suau && Arnau Falagueras

#include"j1App.h"
#include"Collision_Manager.h"
#include"j1Window.h"
#include"j1Map.h"
///////////TEMPORAL
#include "j1Player.h"


Collider_Manager::Collider_Manager() {

}
Collider_Manager::~Collider_Manager() {

}

//Load all colliders from all the object layers with the tag "isColliderLayer" set to true
void Collider_Manager::LoadColliders() 
{
	//for each object layer
	p2List_item<MapObjectGroup*>* itemA;
	itemA = App->map->data.object_layers.start;
	while (itemA != NULL)
	{
		//for each prop in that layer
		p2List_item<object_property*>* isCollider;
		isCollider = itemA->data->properties.start;
		while (isCollider != NULL)
		{
			//if the layer is a collider layer
			if (isCollider->data->name == "isColliderLayer" && isCollider->data->prop_value.value_bool == true) 
			{
				//Load rects and types
				p2List_item<object_struct*>* objects;
				objects = itemA->data->objects.start;
				while (objects != NULL)
				{

					Collider clr;

					clr.collider_rect = objects->data->rect;
					clr.enabled = true;
					
					p2List_item<object_property*>* clr_type;
					clr_type = objects->data->properties.start;
					while (clr_type !=NULL)
					{

						if (clr_type->data->name == "colliderType") 
						{
							if ((p2SString)clr_type->data->prop_value.value_string == "Walkeable")
							{
								clr.collider_type = WALKEABLE;
							}
							else if((p2SString)clr_type->data->prop_value.value_string == "Kill")
							{
								clr.collider_type = KILL;
							}
							else if((p2SString)clr_type->data->prop_value.value_string == "Climb")
							{
								clr.collider_type = CLIMB;
							}
							LOG("%s", clr_type->data->prop_value.value_string);
						}

						clr_type = clr_type->next;
					}


					collider_list.add(clr);


					objects = objects->next;
				}
			}
			isCollider = isCollider->next;
		}
		itemA = itemA->next;
	}





}

//Move an object by movement prediction
void Collider_Manager::MoveObject(SDL_Rect* currentPoint, p2Point<int> increment) 
{

	//Set movement prediction
	SDL_Rect prediction = *currentPoint;
	prediction.x += increment.x;
	prediction.y += increment.y;


	//Set movement direction
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

	//Itinerate all collider objects
	for (int i = 0; i < collider_list.count(); i++)
	{
		SDL_Rect *block = &collider_list[i].collider_rect;
		//Watch if the collider is inside the camera, if not, do not calculate colision
		if ((block->x + block->w) * App->win->GetScale() >= -App->render->camera.x && block->x * App->win->GetScale() <= -App->render->camera.x + App->win->width
			&& (block->y + block->h) * App->win->GetScale() >= -App->render->camera.y && block->y * App->win->GetScale() <= -App->render->camera.y + App->win->height)
		{
			//Is the collider enabled?
			if (collider_list[i].enabled)
			{
				//If it is, check for collisions between it and the object
				if (CheckCollision(prediction, *block))
				{
					//If there is a colision, look collider type
					if (collider_list[i].collider_type == WALKEABLE) 
					{
						//Allow the object to ignore down collisions (player jumping in topo of platform)
						if (allowClippingCollider != nullptr && currentPoint->y < allowClippingCollider->collider_rect.y) 
						{
							allowClippingCollider = nullptr;
						}
						//TODO: Maybe going too fast can cause clipping
						if (&collider_list[i] != allowClippingCollider) 
						{
							//Is the collision inside x and x + w?
							if (prediction.x + prediction.w > block->x && prediction.x < block->x + block->w)
							{
								colisionDetectedY = true;
								//Correct movement or move object in a normal way
								if (prediction.y >= block->y && prediction.y <= block->y + (block->h / 2))
								{
									if (dir == DOWN)
									{
 										currentPoint->y = block->y;
										//TEMPORAL!!!!!!!!!!!!!!!!
										App->player->player.player_not_jumping = true;
									}
								}
								else if (prediction.y + prediction.h < block->y + block->h && prediction.y > block->y + (block->h / 2))
								{
									if (dir == UP)
									{
										//currentPoint->y = block->y + block->h - currentPoint->h;
										allowClippingCollider = &collider_list[i];
									}

								}
							}
							//Is the collision inside y and y + h?
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
					}
					//If collider is type KILL, kill player
					else if(collider_list[i].collider_type == KILL)
					{
						LOG("KILL");
					}
					
				}

			}
		}
	}


	//If no movement correction is needed, therefore there is no collisions, just move the object to the predicted point
	if (!colisionDetectedX)
	{
		currentPoint->x = prediction.x;
	}
	if (!colisionDetectedY)
	{
		currentPoint->y = prediction.y;
	}
}