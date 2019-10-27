//Code by Miquel Suau && Arnau Falagueras

#include"j1App.h"
#include"Collision_Manager.h"
#include"j1Window.h"
#include"j1Map.h"
#include"j1Input.h"
#include "j1Audio.h"
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
						else
						{
							clr.canBeJumped = false;
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
void Collider_Manager::MoveObject(SDL_Rect* currentPoint, p2Point<int> increment, bool isPlayer) 
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
	bool colisionDetectedX = false;
	bool colisionDetectedY = false;
	typeColDetected = false;

	//Itinerate all collider objects
	for (int i = 0; i < collider_list.count(); i++)
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

					//If there is a colision, look collider type
					if ((collider_list[i].collider_type == WALKEABLE && !App->player->player.player_tang_mode) || (collider_list[i].collider_type == TANG && App->player->player.player_tang_mode))
					{
						//Allow the object to ignore down collisions (player jumping in top of platform)
						if (allowClippingCollider != nullptr && currentPoint->y <= allowClippingCollider->collider_rect.y) 
						{
							allowClippingCollider = nullptr;
						}


						if (&collider_list[i] != allowClippingCollider) 
						{
							//Is the collision inside x and x + w?
							if (prediction.x + prediction.w > block->x && prediction.x < block->x + block->w)
							{
								//Correct movement or move object in a normal way
								if (prediction.y >= block->y && prediction.y <= block->y + (block->h / 5) - prediction.h)
								{
									colisionDetectedY = true;
									if (dir == DOWN)
									{
										currentPoint->y = block->y;

										if (isPlayer) 
										{

											if (!App->player->canJump)
												App->player->canJump = true;

											if (App->player->player.player_speed.y >= 2 && App->player->player.col_state == player_colision_state::NONE)
												App->audio->PlayFx(App->player->jump_down_fx);

											App->player->player.player_not_jumping = true;
											App->player->player.player_in_air = false;
										}

									}
								}
								else if (prediction.y + prediction.h < block->y + block->h && prediction.y > block->y + (block->h / 2))
								{
									if (dir == UP)
									{
										colisionDetectedY = true;
										if (isPlayer && App->player->player.col_state == player_colision_state::CLIMBING) 
										{
											colisionDetectedY = false;
											allowClippingCollider = &collider_list[i];
										}

										if (collider_list[i].canBeJumped) 
										{
											allowClippingCollider = &collider_list[i];
										}
										else
										{
											currentPoint->y = block->y + block->h - prediction.h;
										}
									}

								}
							}
							if (prediction.y > block->y && prediction.y + prediction.h < block->y + block->h)
							{
								colisionDetectedX = true;
								////Coliding with the sides of an object
								if (prediction.x <= block->x + block->w)
								{
									if (dir == LEFT)
									{
										if(isPlayer && App->player->player.col_state != player_colision_state::CLIMBING)
											currentPoint->x = block->x + block->w;
									}
								}
								else if (prediction.x + prediction.w >= block->x)
								{
									if (dir == RIGHT)
									{
										if (isPlayer && App->player->player.col_state != player_colision_state::CLIMBING)
											currentPoint->x = block->x - currentPoint->w;
									}
								}

							}
						}
					}

					//If collider is type KILL, kill player
					if(collider_list[i].collider_type == KILL && prediction.y > collider_list[i].collider_rect.y + (collider_list[i].collider_rect.h / 2))
					{
						if (isPlayer && !App->player->player.player_tang_mode && !App->player->player.player_god_mode)
						{
							App->player->Change_Col_State(player_colision_state::DYING);
							typeColDetected = true;
							LOG("KILL");
						}

					}

					//If collider is type Climb, climb
					if(collider_list[i].collider_type == CLIMB)
					{
						typeColDetected = true;
						if (isPlayer && !App->player->player.player_tang_mode && 
							(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || 
							(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->player->player.player_rect.y > collider_list[i].collider_rect.y)) && 
							(App->player->player.player_rect.x + (App->player->player.player_rect.w / 2) > collider_list[i].collider_rect.x
								&& App->player->player.player_rect.x + (App->player->player.player_rect.w / 2) < collider_list[i].collider_rect.x + collider_list[i].collider_rect.w))
						{
							if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && collider_list[i].collider_rect.y + collider_list[i].collider_rect.h > App->player->player.player_rect.y)
							{
								colisionDetectedY = false;
								colisionDetectedX = false;
							}
							App->player->Change_Col_State(player_colision_state::CLIMBING);
							LOG("CLIMB");
						}
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
		if (increment.y > 0) 
		{
			App->player->canJump = false;
		}
	}

	//Reset typeColDetected state
	if (!typeColDetected) 
	{
		App->player->Change_Col_State(player_colision_state::NONE);
	}
}