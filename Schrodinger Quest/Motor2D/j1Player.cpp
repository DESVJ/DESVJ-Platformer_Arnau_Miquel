//Code by Miquel Suau && Arnau Falagueras

#include"j1Player.h"
#include"j1App.h"
#include"j1Render.h"
#include"j1Map.h"
#include"j1Input.h"
#include"j1Textures.h"
#include "Animation.h"
#include"j1Window.h"
#include "j1State_Machine.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "EntityManager.h"

j1Player::j1Player(Types type) : eCreature(Types::player)
{
	name.create("player");
}

// Destructor
j1Player::~j1Player()
{

}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& config)
{

	pugi::xml_document	player_info_file;
	player_info_file.load_file(config.child("load_file").child_value());
	pugi::xml_node player_node = player_info_file.child("map");

	CreateCollider(Collider_Types::PLAYER);

	//player.player_spritesheet = App->tex->Load(player_node.child("imagelayer").child("image").attribute("source").as_string());

	//Animation laoding 
	LoadAnimationFromTMX(&player_node, &idle, "idle");
	LoadAnimationFromTMX(&player_node, &move, "move");
	LoadAnimationFromTMX(&player_node, &jump, "jump");
	LoadAnimationFromTMX(&player_node, &idle_ladder, "idle_ladder");
	LoadAnimationFromTMX(&player_node, &movement_ladder, "movement_ladder");
	LoadAnimationFromTMX(&player_node, &death, "death");
	LoadAnimationFromTMX(&player_node, &down_attack, "down_attack");

	//Sound loading
	LoadSoundFXFromTMX(&player_node, death_fx, "death");
	LoadSoundFXFromTMX(&player_node, jump_down_fx, "jump_down");
	LoadSoundFXFromTMX(&player_node, jump_up_fx, "jump_up");
	LoadSoundFXFromTMX(&player_node, switch_fx, "switch");


	//Var loading
	difference_y = config.child("difference_y").attribute("value").as_int();
	flip = config.child("player_info").attribute("flip").as_bool();
	player.player_not_jumping = config.child("player_info").attribute("not_jumping").as_bool();
	player.player_stop_jumping_up = config.child("player_info").attribute("stop_jumping_up").as_bool();
	player.player_in_air = config.child("player_info").attribute("in_air").as_bool();
	player.player_god_mode = config.child("player_info").attribute("god_mode").as_bool();
	player.player_tang_mode = config.child("player_info").attribute("tang_mode").as_bool();
	alive = config.child("player_info").attribute("alive").as_bool();
	respawn = config.child("player_info").attribute("respawn").as_bool();
	player.player_climbing = config.child("player_info").attribute("climbing").as_bool();
	player.spacebar_pushed = config.child("player_info").attribute("spacebar_pushed").as_bool();
	player.stop_attack = config.child("player_info").attribute("stop_attack").as_bool();
	inputs_out = config.child("inputs_out").attribute("value").as_int();
	actual_state = (state)config.child("actual_state").attribute("value").as_int();
	gravity = config.child("gravity").attribute("value").as_float();
	player.texture_source = config.child("texture_source").child_value();
	maximum_speed= config.child("maximum_speed").attribute("value").as_float();
	animation_created_mov = config.child("animation_created_mov").attribute("value").as_int();
	tangSwitchDeadCheck = config.child("tangSwitchDeadCheck").attribute("value").as_bool();
	canJump = config.child("canJump").attribute("value").as_bool();

	collider->collider_rect.w = config.child("collider_rect").attribute("w").as_int();
	collider->collider_rect.h = config.child("collider_rect").attribute("h").as_int();

	return true;
}

// Called before the first frame
bool j1Player::Start()
{
	//Load player spawn position
	MoveToSpawn();

	//Load player spritesheet
	player.player_spritesheet = App->tex->Load(player.texture_source.GetString());

	return true;
}

bool j1Player::PreUpdate() 
{
	CheckInputs(player.player_god_mode, player.player_tang_mode, player.player_not_jumping, player.spacebar_pushed, canJump, tangSwitchDeadCheck,player.stop_attack,inputs_out, speed.y, actual_state, input_in, input_out, player.col_state, collider->collider_rect);

	if (respawn == true)
		Start_F3();

	//Move camera to new player position
	App->render->MoveCameraToPointInsideLimits({ collider->collider_rect.x + (collider->collider_rect.w / 2), collider->collider_rect.y });

	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	player.player_climbing = false;
	bool reset_animation = CheckState(inputs_out, actual_state, input_in, input_out);
	Animation* current_animation = ExecuteState(speed, actual_state, reset_animation, player.player_climbing, alive, player.player_god_mode, player.player_in_air, player.player_stop_jumping_up, flip);
	//Animation* current_animation = &slide;
	if (reset_animation == true) 
	{
		current_animation->Reset();
	}

	SDL_Rect current_frame = current_animation->GetCurrentFrame(dt);
	// Check if player is dead and dead animation is finished
	if (alive == false) 
	{
		if (current_animation->GetFinished() == 1) 
		{
			respawn = true;
		}
	}

	// Check if player is attacking and attack animation is finished
	if (actual_state == S_DOWN_ATTACK && current_animation->GetFinished() == 1)
	{
		player.stop_attack = true;
	}

	//Slide mec
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_DOWN && speed.x != 0)
	{
		if (!player.isSliding) 
		{
			player.isSliding = true;
			if (flip == SDL_FLIP_NONE) 
			{
				speed.x += 8;
			}
			else
			{
				speed.x -= 8;
			}
		}
	}
	if (player.isSliding)
	{
		if (flip == SDL_FLIP_NONE)
		{
			speed.x -= 0.4f;
			if (speed.x <= 0) 
			{
				player.isSliding = false;
			}
		}
		else
		{
			speed.x += 0.4f;
			if (speed.x >= 0)
			{
				player.isSliding = false;
			}
		}

	}
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_UP ) 
	{

		speed.x = 0;
		player.isSliding = false;
	}


	// Change if the sprite if intangible
	//if (player.player_tang_mode == true)
	//	current_frame.y += difference_y;

	//Calculate animation offset
	if (position_rect.w != 0) 
	{
		animation_created_mov = collider->collider_rect.w - current_frame.w;
	}

	//Update player rect
	position_rect.w = current_frame.w;
	position_rect.h = -current_frame.h;

	//Increment player speed by gravity
	if (player.player_stop_jumping_up == true && player.player_god_mode == false && alive == true && player.player_climbing == false)
	{
		if (speed.y < maximum_speed)
			speed.y += gravity;
	}

	//Move player
	if (player.col_state != player_colision_state::DYING) 
	{
		App->colliders->MoveObject(&collider->collider_rect, { (int)round(speed.x * 60 * dt) , 0}, this);
		if(dt>0.5)App->colliders->MoveObject(&collider->collider_rect, { 0, (int)round(speed.y) }, this);
		else {
			if (dt > 0.02 && player.player_god_mode == false)
				App->colliders->MoveObject(&collider->collider_rect, { 0, (int)round(speed.y * 35 * dt) }, this);
			App->colliders->MoveObject(&collider->collider_rect, { 0, (int)round(speed.y * 60 * dt) }, this);
		}
	}


	//----------------------------------------------------------------------//

	//Show colliders
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) 
	{
		App->input->is_Debug_Mode = !App->input->is_Debug_Mode;
	}

	//God mode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		player.player_god_mode = !player.player_god_mode;
		player.player_not_jumping = true;
		player.spacebar_pushed = false;
	}

	//----------------------------------------------------------------------//
	//Jump control check
	if (dt < 0.02f) {
		if ((speed.y * 60 * dt < -maximum_speed * (60 * dt) && player.player_god_mode == false) || player.spacebar_pushed == false)
		{
			player.player_stop_jumping_up = true;
			player.spacebar_pushed = false;
		}
	}
	else {
		if ((speed.y / (30 * dt) < -maximum_speed / (60 * dt) && player.player_god_mode == false) || player.spacebar_pushed == false)
		{
			player.player_stop_jumping_up = true;
			player.spacebar_pushed = false;
		}
	}
	//Update player graphics by collider position
	position_rect.x = collider->collider_rect.x + (animation_created_mov / 2);
	position_rect.y = collider->collider_rect.y;


	//Check if player is under the map and kill it
	if (position_rect.y + position_rect.h > killLimit && !respawn)
	{
		Change_Col_State(player_colision_state::DYING);
	}

	//Flip player 
	if (flip == false && speed.x < 0)
		flip = true;
	else if (flip == true && speed.x > 0)
		flip = false;

	//Render player GFX
	App->render->Blit(player.player_spritesheet, position_rect.x, position_rect.y - current_frame.h, &current_frame, flip);

	//Render player colider in debug mode
	if (App->input->is_Debug_Mode) 
	{
		App->render->DrawQuad(App->render->followMinRect, 255, 210, 78, 50);
	}

	//Clear output array
	for (int i = 0; i < inputs_out; i++)
		input_out[i] = O_NONE;

	inputs_out = 0;
	input_in = I_NONE;

	//Check map ending
	if (App->colliders->CheckCollision(position_rect, App->map->end_point) == true) 
	{
		App->map->map_id++;
		if (App->map->map_id > MAX_NUMBER_MAPS)
			App->map->map_id = 1;
		App->scene->Load_Map_By_Name(App->map->GetSourceFromID(App->map->map_id).GetString());
	}

	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(player.player_spritesheet);
	App->colliders->collider_list.clear();
	return true;
}

//Start level from spanw
void j1Player::Start_F3()
{

	MoveToSpawn();
	if (input_in == I_DEAD)input_in = I_NONE;
	Change_Col_State(player_colision_state::NONE);

	//Restart player vars
	flip = false;
	player.player_not_jumping = true;
	player.player_god_mode = false;
	player.player_tang_mode = false;
	alive = true;
	respawn = false;
	player.player_climbing = false;
	player.spacebar_pushed = false;
	player.stop_attack = false;
	inputs_out = 0;
	actual_state = (state)1;

	//Restart camera position
	//App->render->MoveCameraToPointInsideLimits({ player.player_rect.x + (player.player_rect.w / 2), player.player_rect.y });
}

void j1Player::Change_Col_State(player_colision_state state)
{
	player.col_state = state;
}

bool j1Player::Save(pugi::xml_node& data)const 
{

	//Save all vars to XML fileup
	pugi::xml_node player_node = data.append_child("player_info");
	player_node.append_child("position");
	player_node.child("position").append_attribute("x") = collider->collider_rect.x;
	player_node.child("position").append_attribute("y") = collider->collider_rect.y;
	player_node.child("position").append_attribute("w") = position_rect.w;
	player_node.child("position").append_attribute("h") = position_rect.h;
	player_node.append_child("speed");
	player_node.child("speed").append_attribute("x") = speed.x;
	player_node.child("speed").append_attribute("y") = speed.y;
	player_node.append_attribute("flip") = flip;
	player_node.append_attribute("not_jumping") = player.player_not_jumping;
	player_node.append_attribute("stop_jumping_up") = player.player_stop_jumping_up;
	player_node.append_attribute("in_air") = player.player_in_air;
	player_node.append_attribute("spacebar_pushed") = player.spacebar_pushed;
	player_node.append_attribute("god_mode") = player.player_god_mode;
	player_node.append_attribute("tang_mode") = player.player_tang_mode;
	player_node.append_attribute("alive") = alive;
	player_node.append_attribute("respawn") = respawn;
	player_node.append_attribute("climbing") = player.player_climbing;
	player_node.append_attribute("stop_attack") = player.stop_attack;
	player_node.append_attribute("col_state") = player.col_state;
	player_node.append_attribute("actual_state") = actual_state;

	return true;
}

bool j1Player::Load(pugi::xml_node& data) 
{
	//Load all vars to xml file
	collider->collider_rect.x = data.child("player_info").child("position").attribute("x").as_int();
	collider->collider_rect.y = data.child("player_info").child("position").attribute("y").as_int();
	position_rect.w = data.child("player_info").child("position").attribute("w").as_int();
	position_rect.h = data.child("player_info").child("position").attribute("h").as_int();
	speed.x = data.child("player_info").child("speed").attribute("x").as_float();
	speed.y = data.child("player_info").child("speed").attribute("y").as_float();
	flip = data.child("player_info").attribute("flip").as_bool();
	player.player_not_jumping = data.child("player_info").attribute("not_jumping").as_bool();
	player.player_stop_jumping_up = data.child("player_info").attribute("stop_jumping_up").as_bool();
	player.player_in_air = data.child("player_info").attribute("in_air").as_bool();
	player.spacebar_pushed = data.child("player_info").attribute("spacebar_pushed").as_bool();
	player.player_god_mode = data.child("player_info").attribute("god_mode").as_bool();
	player.player_tang_mode = data.child("player_info").attribute("tang_mode").as_bool();
	alive = data.child("player_info").attribute("alive").as_bool();
	respawn = data.child("player_info").attribute("respawn").as_bool();
	player.player_climbing = data.child("player_info").attribute("climbing").as_bool();
	player.stop_attack = data.child("player_info").attribute("stop_attack").as_bool();
	player.col_state = (player_colision_state)data.child("player_info").attribute("col_state").as_int();
	actual_state = (state)data.child("player_info").attribute("actual_state").as_int();

	return true;
}

void j1Player::LoadSoundFXFromTMX(pugi::xml_node* sound_node, unsigned int& fx, const char* name) 
{
	//Load sound from xml object groups
	pugi::xml_node objectgroup;
	pugi::xml_node *correctNodeGroup = nullptr;
	for (objectgroup = sound_node->child("objectgroup"); objectgroup; objectgroup = objectgroup.next_sibling("objectgroup"))
	{
		if (strcmp(objectgroup.attribute("name").as_string(), "Player_FXS")==0) break;
	}
	for (pugi::xml_node obj_prop = objectgroup.child("properties").first_child(); obj_prop; obj_prop = obj_prop.next_sibling("property")) {
		if ((p2SString)obj_prop.attribute("name").value() == (p2SString)name)
		{
			fx=App->audio->LoadFx(obj_prop.attribute("value").as_string());
			break;
		}
	}
}

void j1Player::MoveToSpawn()
{
	//Respawn all creatures
	App->entity_manager->RespawnCreatures();


	//Reload music
	p2List_item<MapObjectGroup*>* objects_map;
	objects_map = App->map->data.object_layers.start;
	while (objects_map != NULL)
	{
		if (objects_map->data->name == "Music && Sound")
		{
			//Restart music
			if (alive == true)App->map->PrepareMusicSource(objects_map);
			else App->map->PrepareMusicSource(objects_map, true);
		}
		objects_map = objects_map->next;
	}	
}

p2Point<bool> j1Player::OnCollision(Collider* in_collider, SDL_Rect prediction, SDL_Rect* block, Direction dir, p2Point<bool> prev_res)
{
	//If there is a colision, look collider type
	if ((in_collider->collider_type == WALKEABLE && !player.player_tang_mode) || (in_collider->collider_type == TANG && player.player_tang_mode))
	{
		//Allow the object to ignore down collisions (player jumping in top of platform)
		if (allowClippingCollider != nullptr && collider->collider_rect.y <= allowClippingCollider->collider_rect.y)
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
						collider->collider_rect.y = block->y;

						if (speed.y >= 8 && canJump == false && player.col_state == player_colision_state::NONE)
							App->audio->PlayFx(jump_down_fx);

						if (!canJump)
							canJump = true;

						player.player_not_jumping = true;
						player.player_in_air = false;
	
					}
				}
				else if (prediction.y + prediction.h < block->y + block->h && prediction.y > block->y + (block->h / 2))
				{
					if (dir == UP)
					{
						prev_res.y = true;
						if (player.col_state == player_colision_state::CLIMBING)
						{
							prev_res.y = false;
							allowClippingCollider = in_collider;
						}

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
			if (prediction.y > block->y && prediction.y + prediction.h < block->y + block->h)
			{
				prev_res.x = true;
				////Coliding with the sides of an object
				if (prediction.x <= block->x + block->w)
				{
					if (dir == LEFT)
					{
						if (player.col_state != player_colision_state::CLIMBING)
							collider->collider_rect.x = block->x + block->w;
					}
				}
				else if (prediction.x + prediction.w >= block->x)
				{
					if (dir == RIGHT)
					{
						if (player.col_state != player_colision_state::CLIMBING)
							collider->collider_rect.x = block->x - collider->collider_rect.w;
					}
				}

			}
		}
	}

	//If collider is type KILL, kill player
	if (in_collider->collider_type == KILL && prediction.y > in_collider->collider_rect.y + (in_collider->collider_rect.h / 2))
	{
		if (!player.player_tang_mode && !player.player_god_mode)
		{
			Change_Col_State(player_colision_state::DYING);
			typeColDetected = true;
			LOG("KILL");
		}

	}

	//If collider is type Climb, climb
	if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT))
	{
		if (in_collider->collider_type == CLIMB && !player.player_tang_mode &&
			(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT ||
			(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && position_rect.y > in_collider->collider_rect.y)) &&
				(position_rect.x + (position_rect.w / 2) > in_collider->collider_rect.x
					&& position_rect.x + (position_rect.w / 2) < in_collider->collider_rect.x + in_collider->collider_rect.w))
		{
			typeColDetected = true;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && in_collider->collider_rect.y + in_collider->collider_rect.h > collider->collider_rect.y)
			{
				prev_res = {false, false};
			}
			Change_Col_State(player_colision_state::CLIMBING);
			LOG("CLIMB");
		}
		else
		{
			Change_Col_State(player_colision_state::NONE);
			typeColDetected = false;
		}
	}
	else if(in_collider->collider_type == CLIMB)
	{
		Change_Col_State(player_colision_state::NONE);
		typeColDetected = false;
	}

	if (in_collider->collider_type == Collider_Types::ENEMY)
	{
		if (actual_state == state::S_DOWN_ATTACK || actual_state == state::S_DOWN_ATTACK_JUMP) 
		{
			if ((flip == SDL_FLIP_NONE && in_collider->collider_rect.x + in_collider->collider_rect.w >= collider->collider_rect.x) || (flip == SDL_FLIP_HORIZONTAL && in_collider->collider_rect.x - in_collider->collider_rect.w <= collider->collider_rect.x + collider->collider_rect.w))
			{
				for (unsigned int i = 0; i < App->entity_manager->entities.count(); i++)
				{
					Entity* ent = App->entity_manager->entities[i];
					if (ent->entity_type == Types::enemy_ground || ent->entity_type == Types::enemy_air)
					{
						eCreature* creature = (eCreature*)App->entity_manager->entities[i];
						if (creature->collider == in_collider) 
						{
							in_collider->collider_rect.x = -100;
							in_collider->collider_rect.y = -100;
							creature->alive = false;
						}
					}
				}
			}
		}
		else if (!player.player_god_mode)
		{
			Change_Col_State(player_colision_state::DYING);
			typeColDetected = true;
			LOG("KILL");
		}
	}

	return prev_res;
}
void j1Player::AfterCollision(p2Point<bool> col_result, SDL_Rect prediction, p2Point<int> increment)
{
	//If no movement correction is needed, therefore there is no collisions, just move the object to the predicted point
	if (col_result.x == false)
	{
		collider->collider_rect.x = prediction.x;
	}
	if (col_result.y == false)
	{
		collider->collider_rect.y = prediction.y;
		if (increment.y > 0)
		{
			canJump = false;
		}
	}

	if (player.col_state != player_colision_state::DYING) 
	{
		//Reset typeColDetected state
		if (typeColDetected == false)
		{
			Change_Col_State(player_colision_state::NONE);
		}
	}
}

pugi::xml_node j1Player::LoadAnimationFromTMX(pugi::xml_node* animation_node, Animation* anim, const char* name)
{
	pugi::xml_node res = eCreature::LoadAnimationFromTMX(animation_node, anim, name);

	if (player.minPlayerHeight == 0)
	{
		player.minPlayerHeight = -res.attribute("height").as_int();
	}
	else if (player.minPlayerHeight > res.attribute("height").as_int())
	{
		player.minPlayerHeight = -res.attribute("height").as_int();
	}
	return res;
}