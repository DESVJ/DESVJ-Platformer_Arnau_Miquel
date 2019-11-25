//Code by Miquel Suau && Arnau Falagueras

#include"j1App.h"
#include"Collision_Manager.h"
#include"j1Window.h"
#include"j1Map.h"
#include"j1Input.h"
#include "j1Audio.h"
#include "EntityManager.h"
#include "j1Player.h"


Collider_Manager::Collider_Manager() 
{

}
Collider_Manager::~Collider_Manager() 
{

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
						//Load collider type
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
							else if ((p2SString)clr_type->data->prop_value.value_string == "Tang")
							{
								clr.collider_type = TANG;
							}
						}

						//Load collider canBeJumped
						if (clr_type->data->name == "canBeJumpThrough")
						{
							clr.canBeJumped = clr_type->data->prop_value.value_bool;
						}

						clr_type = clr_type->next;
					}
					
					//Add collider to list
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
void Collider_Manager::MoveObject(SDL_Rect* currentPoint, p2Point<int> increment, Creature* s_mod)
{

	//Set movement prediction
	SDL_Rect prediction = *currentPoint;
	prediction.x += increment.x;
	prediction.y += increment.y;

	//Set movement direction
	Direction dir = NONE_DIRECTION;
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


	//Init coollision bools
	p2Point<bool> result = { false, false };
	s_mod->typeColDetected = false;

	//Itinerate all collider objects
	for (unsigned int i = 0; i < collider_list.count(); i++)
	{
		SDL_Rect *block = &collider_list[i].collider_rect;

		//Watch if the collider is inside the camera, if not, do not calculate colision
		if (App->map->Culling_Check(block->x, block->y, {0, 0, block->w, block->h}, 1))
		{
			//Is the collider enabled?
			if (collider_list[i].enabled)
			{
				//If it is, check for collisions between it and the object
				if (CheckCollision(prediction, *block))
				{
					result = s_mod->OnCollision(&collider_list[i], prediction, block, dir, result);
				}
			}
		}
	}
	s_mod->AfterCollision(result, prediction, increment);
}


bool Collider_Manager::CheckCollision(const SDL_Rect& rect, const SDL_Rect& r)
{
	bool detectedX = true;
	bool detectedY = true;

	if ((rect.x + rect.w) < r.x || (r.x + r.w) < rect.x)
	{
		detectedX = false;
	}

	if (rect.y < r.y || (rect.y + rect.h) > r.y + r.h)
	{
		detectedY = false;
	}

	return detectedX && detectedY;
}

bool Collider_Manager::CheckAbsoluteCollision(const SDL_Rect& rect, const SDL_Rect& r)
{

	bool detectedX = false;
	bool detectedY = false;

	//Check if rect is full inside of r
	if (rect.x > r.x && rect.x + rect.w < r.x + r.w)
	{
		detectedX = true;
	}

	if (rect.y < r.y + r.h && rect.y + rect.h > r.y)
	{
		detectedY = true;
	}

	return detectedX && detectedY;
}