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
//////////////TEMPORAL
#include "j1Scene.h"

j1Player::j1Player() : j1Module()
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


	player_info_file.load_file(config.child("load_file").child_value());
	pugi::xml_node player_node = player_info_file.child("map");

	player.player_spritesheet = App->tex->Load(player_node.child("imagelayer").child("image").attribute("source").as_string());

	

	//LoadAnimationFromTMX(&player_node, &idle, "idle");
	LoadAnimationFromTMX(&player_node, &idle, "idle");
	LoadAnimationFromTMX(&player_node, &run, "run");
	LoadAnimationFromTMX(&player_node, &jump, "jump");
	LoadAnimationFromTMX(&player_node, &idle_ladder, "idle_ladder");
	LoadAnimationFromTMX(&player_node, &movement_ladder, "movement_ladder");
	LoadAnimationFromTMX(&player_node, &death, "death");

	//LoadSoundFXFromTMX
	LoadSoundFXFromTMX(&player_node, death_fx, "death");
	LoadSoundFXFromTMX(&player_node, jump_down_fx, "jump_down");
	LoadSoundFXFromTMX(&player_node, jump_up_fx, "jump_up");
	LoadSoundFXFromTMX(&player_node, switch_fx, "switch");


	difference_y = config.child("difference_y").attribute("value").as_int();
	player.player_flip = config.child("player_info").attribute("flip").as_bool();
	player.player_not_jumping = config.child("player_info").attribute("not_jumping").as_bool();
	player.player_stop_jumping_up = config.child("player_info").attribute("stop_jumping_up").as_bool();
	player.player_god_mode = config.child("player_info").attribute("god_mode").as_bool();
	player.player_tang_mode = config.child("player_info").attribute("tang_mode").as_bool();
	player.player_alive = config.child("player_info").attribute("alive").as_bool();
	player.player_respawn = config.child("player_info").attribute("respawn").as_bool();
	player.player_climbing = config.child("player_info").attribute("climbing").as_bool();
	player.spacebar_pushed = config.child("player_info").attribute("spacebar_pushed").as_bool();
	inputs_out = config.child("inputs_out").attribute("value").as_int();
	actual_state = (state)config.child("actual_state").attribute("value").as_int();
	gravity = config.child("gravity").attribute("value").as_int();
	player.texture_source = config.child("texture_source").child_value();
	maximum_speed= config.child("maximum_speed").attribute("value").as_int();
	//LoadAnimation(&config.child("animations"), &run, "run");
	//LoadAnimation(&config.child("animations"), &jump, "jump");
	//LoadAnimation(&config.child("animations"), &idle_ladder, "idle_ladder");
	//LoadAnimation(&config.child("animations"), &movement_ladder, "movement_ladder");
	//LoadAnimation(&config.child("animations"), &death, "death");

	return true;
}

// Called before the first frame
bool j1Player::Start()
{

	p2List_item<MapObjectGroup*>* objects_map;
	objects_map = App->map->data.object_layers.start;
	while (objects_map != NULL)
	{

		if (objects_map->data->name == "SpawnPoint"&& objects_map->data->properties.start->data->prop_value.value_bool == true) {
			player.player_rect.x = objects_map->data->objects.start->data->rect.x;
			player.player_rect.y = objects_map->data->objects.start->data->rect.y;
			player.player_rect.w = objects_map->data->objects.start->data->rect.w;
			player.player_rect.h = objects_map->data->objects.start->data->rect.h;
		}
		objects_map = objects_map->next;
	}
	//player.player_rect.x = App->map->data.tile_width * 5;
	//player.player_rect.y = App->map->data.tile_width * 8;
	//player.player_rect.w = 0;
	//player.player_rect.h = 0;


	player.player_spritesheet = App->tex->Load(player.texture_source.GetString());

	return true;
}

bool j1Player::PreUpdate() {
	CheckInputs(player.player_god_mode, player.player_not_jumping, inputs_out, player.player_speed.y, actual_state, input_in, input_out, player.col_state);
	if (player.player_respawn == true)Start_F3();
	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	player.player_climbing = false;
	bool reset_animation = CheckState(inputs_out, actual_state, input_in, input_out);
	Animation* current_animation = ExecuteState(player.player_speed, actual_state, reset_animation);
	if (reset_animation == true) {
		current_animation->Reset();
	}
	SDL_Rect current_frame = current_animation->GetCurrentFrame();
	// Check if player is dead and dead animation is finished
	if (player.player_alive == false) {
		if (current_animation->GetFinished() == 1) {
		//	current_animation->Reset();
			player.player_respawn = true;
		}
	}
	// Change the sprite if intangible
	if (player.player_tang_mode == true)current_frame.y += difference_y;

	if (player.player_rect.w != 0) {

		int animation_created_mov = player.player_rect.w - current_frame.w;
		if(animation_created_mov != 0)  
			App->colliders.MoveObject(&player.player_rect, { animation_created_mov/2, 0 }, true);
			//App->colliders.MoveObject(&player.player_rect, {animation_created_mov / 2, 0}, true); Better divided by 2 but it breaks colisions in right walls
	}

	//TODO: Smooth camera follow
	player.player_rect.w = current_frame.w;
	player.player_rect.h = -current_frame.h;



	App->colliders.MoveObject(&player.player_rect, { player.player_speed.x , 0}, true);
	App->colliders.MoveObject(&player.player_rect, { 0, player.player_speed.y }, true);
	//TEMPORAL//
	App->colliders.Correct(&player.player_rect);
	if (player.player_rect.y - player.player_rect.h > App->render->limitNegY)
	{
		player.player_alive = false;
	}
	if (/*current_animation != &jump*/player.player_stop_jumping_up == true && player.player_god_mode == false/* && player.player_alive == true*/ && player.player_climbing == false)
	{
		//TODO: Falling looks wird on high falls
		//App->colliders.MoveObject(&player.player_rect, { 0, 4}, true);
		if(player.player_speed.y<maximum_speed)player.player_speed.y += gravity;
	}

	//SHOW COLLIDERS
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) 
	{
		App->input->is_Debug_Mode = !App->input->is_Debug_Mode;
	}
	//GOD MODE
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		player.player_god_mode = !player.player_god_mode;
		player.player_not_jumping = true;
		player.spacebar_pushed = false;
	}



	if ((player.player_speed.y < -maximum_speed && player.player_god_mode == false)||player.spacebar_pushed==false) {
		player.player_stop_jumping_up = true;
		player.spacebar_pushed = false;
	}
	/*if (current_animation == &jump && player.player_stop_jumping_up == true && player.player_god_mode == false) {
		//player.player_speed.y += gravity;
	}*/



	//This must be debug mode only
	//if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	//	App->colliders.MoveObject(&player.player_rect, {1, 0});
	//if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	//	App->colliders.MoveObject(&player.player_rect, { -1, 0 });
	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	//	App->colliders.MoveObject(&player.player_rect, { 0, -1 });
	//if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//	App->colliders.MoveObject(&player.player_rect, { 0, 1 });


	//TODO: THIS IS TEMPORAL, WE NEED A SMOOTHER FOLLOW
	App->render->MoveCameraToPointInsideLimits({player.player_rect.x + (player.player_rect.w / 2), player.player_rect.y});


	//App->render->DrawQuad({ player.player_position.x, player.player_position.y, App->map->data.tile_width, App->map->data.tile_height}, 255, 255, 255, 255);
	if (player.player_flip == false && player.player_speed.x < 0)player.player_flip = true;
	else if (player.player_flip == true && player.player_speed.x > 0)player.player_flip = false;
	App->render->Blit(player.player_spritesheet, player.player_rect.x, player.player_rect.y - current_frame.h, &current_frame, player.player_flip);
	if(player.player_god_mode)
		App->render->DrawQuad({ player.player_rect.x, player.player_rect.y, player.player_rect.w, player.player_rect.h }, MAX_COLOR_NUMBER, MAX_COLOR_NUMBER, MAX_COLOR_NUMBER, 55);
	for (int i = 0; i < inputs_out; i++)input_out[i] = O_NONE;
	inputs_out = 0;
	input_in = I_NONE;
	///////TEMPORAL
	if (App->colliders.CheckCollision(player.player_rect, App->map->end_point) == true) {
		App->map->map_id++;
		if (App->map->map_id > MAX_NUMBER_MAPS)App->map->map_id = 1;
		App->scene->Load_Map_By_Name(App->map->GetSourceFromID(App->map->map_id).GetString());
	}
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(player.player_spritesheet);
	App->colliders.collider_list.clear();
	return true;
}

void j1Player::Start_F3() {
	p2List_item<MapObjectGroup*>* objects_map;
	objects_map = App->map->data.object_layers.start;
	while (objects_map != NULL) 
	{
		if (objects_map->data->name == "SpawnPoint"){
			p2List_item<object_property*>* isSpawn;
			isSpawn = objects_map->data->properties.start;
			while (isSpawn != NULL)
			{
				if (isSpawn->data->name == "isSpawn"&&isSpawn->data->prop_value.value_bool == true)
				{
					player.player_rect.x = objects_map->data->objects.start->data->rect.x;
					player.player_rect.y = objects_map->data->objects.start->data->rect.y;
					player.player_rect.w = objects_map->data->objects.start->data->rect.w;
					player.player_rect.h = objects_map->data->objects.start->data->rect.h;
				}
				isSpawn = isSpawn->next;
			}
		}
		else if (objects_map->data->name == "Music && Sound")
		{
			if(player.player_alive==true)App->map->PrepareMusicSource(objects_map);
			else App->map->PrepareMusicSource(objects_map, true);
		}
		objects_map=objects_map->next;
	}
	if (input_in == I_DEAD)input_in = I_NONE;
	App->player->Change_Col_State(player_colision_state::NONE);
	pugi::xml_node config = App->config_file.child("config").child("player");
	player.player_flip = config.child("player_info").attribute("flip").as_bool();
	player.player_not_jumping = config.child("player_info").attribute("not_jumping").as_bool();
	player.player_stop_jumping_up = config.child("player_info").attribute("stop_jumping_up").as_bool();
	player.player_god_mode = config.child("player_info").attribute("god_mode").as_bool();
	player.player_tang_mode = config.child("player_info").attribute("tang_mode").as_bool();
	player.player_alive = config.child("player_info").attribute("alive").as_bool();
	player.player_respawn = config.child("player_info").attribute("respawn").as_bool();
	player.player_climbing = config.child("player_info").attribute("climbing").as_bool();
	player.spacebar_pushed = config.child("player_info").attribute("spacebar_pushed").as_bool();
	inputs_out = config.child("inputs_out").attribute("value").as_int();
	actual_state = (state)config.child("actual_state").attribute("value").as_int();

}

//void j1Player::LoadAnimation(pugi::xml_node* animation_node, Animation* anim, const char* name)
//{
//
//
//	pugi::xml_node animation_set = animation_node->child(name);
//
//	for (pugi::xml_node subNode = animation_set.child("frame"); subNode; subNode = subNode.next_sibling("frame"))
//	{
//		SDL_Rect sect = { subNode .attribute("x").as_int(), subNode.attribute("y").as_int()
//		,subNode.attribute("w").as_int() , subNode.attribute("h").as_int() };
//		anim->PushBack(sect);
//	}
//
//	anim->speed = animation_set.child("speed").attribute("value").as_float();
//
//}

void j1Player::LoadAnimationFromTMX(pugi::xml_node* animation_node, Animation* anim, const char* name)
{

	pugi::xml_node objectgroup;
	pugi::xml_node *correctNodeGroup = nullptr;
	for (objectgroup = animation_node->child("objectgroup"); objectgroup; objectgroup = objectgroup.next_sibling("objectgroup"))
	{

		for (pugi::xml_node obj_prop = objectgroup.child("properties").first_child(); obj_prop; obj_prop = obj_prop.next_sibling("property"))
		{
			if ((p2SString)obj_prop.attribute("value").value() == (p2SString)name)
			{
				correctNodeGroup = &objectgroup;
				if ((p2SString)obj_prop.next_sibling().attribute("name").as_string() == (p2SString)"speed")
					anim->speed = obj_prop.next_sibling().attribute("value").as_float();
				break;
			}
		}
		if (correctNodeGroup != nullptr)
			break;

	}


	if (correctNodeGroup) 
	{
		for (pugi::xml_node obj = correctNodeGroup->child("object"); obj; obj = obj.next_sibling("object"))
		{
			if (obj.child("properties").child("property").attribute("value").as_bool() == true) 
			{
				anim->PushBack({ obj.attribute("x").as_int(),
				obj.attribute("y").as_int(),
				obj.attribute("width").as_int(),
				obj.attribute("height").as_int() });

				if (player.minPlayerHeight == 0) 
				{
					player.minPlayerHeight = -obj.attribute("height").as_int();
				}
				else if (player.minPlayerHeight > obj.attribute("height").as_int()) 
				{
					player.minPlayerHeight = -obj.attribute("height").as_int();
				}


			}
		}
	}
}

void j1Player::Change_Col_State(player_colision_state state)
{
	player.col_state = state;
}

bool j1Player::Save(pugi::xml_node& data)const {
	pugi::xml_node player_node = data.append_child("player_info");
	player_node.append_child("position");
	player_node.child("position").append_attribute("x") = player.player_rect.x;
	player_node.child("position").append_attribute("y") = player.player_rect.y;
	player_node.child("position").append_attribute("w") = player.player_rect.w;
	player_node.child("position").append_attribute("h") = player.player_rect.h;
	player_node.append_child("speed");
	player_node.child("speed").append_attribute("x") = player.player_speed.x;
	player_node.child("speed").append_attribute("y") = player.player_speed.y;
	player_node.append_attribute("flip") = player.player_flip;
	player_node.append_attribute("not_jumping") = player.player_not_jumping;
	player_node.append_attribute("stop_jumping_up") = player.player_stop_jumping_up;
	player_node.append_attribute("spacebar_pushed") = player.spacebar_pushed;
	player_node.append_attribute("god_mode") = player.player_god_mode;
	player_node.append_attribute("tang_mode") = player.player_tang_mode;
	player_node.append_attribute("alive") = player.player_alive;
	player_node.append_attribute("respawn") = player.player_respawn;
	player_node.append_attribute("climbing") = player.player_climbing;
	player_node.append_attribute("col_state") = player.col_state;
	player_node.append_attribute("actual_state") = actual_state;
	return true;
}

bool j1Player::Load(pugi::xml_node& data) {
	player.player_rect.x = data.child("player_info").child("position").attribute("x").as_int();
	player.player_rect.y = data.child("player_info").child("position").attribute("y").as_int();
	player.player_rect.w = data.child("player_info").child("position").attribute("w").as_int();
	player.player_rect.h = data.child("player_info").child("position").attribute("h").as_int();
	player.player_speed.x = data.child("player_info").child("speed").attribute("x").as_int();
	player.player_speed.y = data.child("player_info").child("speed").attribute("y").as_int();
	player.player_flip = data.child("player_info").attribute("flip").as_bool();
	player.player_not_jumping = data.child("player_info").attribute("not_jumping").as_bool();
	player.player_stop_jumping_up = data.child("player_info").attribute("stop_jumping_up").as_bool();
	player.spacebar_pushed = data.child("player_info").attribute("spacebar_pushed").as_bool();
	player.player_god_mode = data.child("player_info").attribute("god_mode").as_bool();
	player.player_tang_mode = data.child("player_info").attribute("tang_mode").as_bool();
	player.player_alive = data.child("player_info").attribute("alive").as_bool();
	player.player_respawn = data.child("player_info").attribute("respawn").as_bool();
	player.player_climbing = data.child("player_info").attribute("climbing").as_bool();
	player.col_state = (player_colision_state)data.child("player_info").attribute("col_state").as_int();
	actual_state = (state)data.child("player_info").attribute("actual_state").as_int();
	return true;
}

void j1Player::LoadSoundFXFromTMX(pugi::xml_node* sound_node, unsigned int& fx, const char* name) {
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