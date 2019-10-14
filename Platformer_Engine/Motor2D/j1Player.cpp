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

j1Player::j1Player() : j1Module()
{
	name.create("player");

	///////TEMPORAL
	difference_y = 256;
}

// Destructor
j1Player::~j1Player()
{

}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& config)
{


	player_info_file.load_file("textures/player.tmx");
	pugi::xml_node player_node = player_info_file.child("map");

	player.player_spritesheet = App->tex->Load(player_node.child("imagelayer").child("image").attribute("source").as_string());

	

	//LoadAnimationFromTMX(&player_node, &idle, "idle");
	LoadAnimationFromTMX(&player_node, &idle, "idle");
	LoadAnimationFromTMX(&player_node, &run, "run");
	LoadAnimationFromTMX(&player_node, &jump, "jump");
	LoadAnimationFromTMX(&player_node, &idle_ladder, "idle_ladder");
	LoadAnimationFromTMX(&player_node, &movement_ladder, "movement_ladder");
	LoadAnimationFromTMX(&player_node, &death, "death");

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
	inputs_out = 0;
	actual_state = S_IDLE;

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
	player.player_flip = false;
	player.player_not_jumping = true;
	player.player_god_mode = false;
	player.player_tang_mode = false;

	player.player_spritesheet =  App->tex->Load("textures/Player_SpriteSheet.png");

	return true;
}

bool j1Player::PreUpdate() {
	CheckInputs(player.player_god_mode, player.player_not_jumping, inputs_out, player.player_speed.y, actual_state, input_in, input_out);
	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	bool reset_animation = CheckState(inputs_out, actual_state, input_in, input_out);
	Animation* current_animation = ExecuteState(player.player_speed, actual_state, reset_animation);
	if (reset_animation == true) {
		current_animation->Reset();
	}
	SDL_Rect current_frame = current_animation->GetCurrentFrame();
	// Change the sprite if intangible
	if (player.player_tang_mode == true)current_frame.y += difference_y;

	if (player.player_rect.w != 0) {

		int animation_created_mov = player.player_rect.w - current_frame.w;
		if(animation_created_mov != 0)  
			App->colliders.MoveObject(&player.player_rect, {animation_created_mov, 0});
	}

	//TODO: Smooth camera follow
	player.player_rect.w = current_frame.w;
	player.player_rect.h = -current_frame.h;



	App->colliders.MoveObject(&player.player_rect, { player.player_speed.x , 0});
	App->colliders.MoveObject(&player.player_rect, { 0, player.player_speed.y });
	if (current_animation != &jump&&player.player_god_mode == false)
	{
		//TODO: Falling looks wird on high falls
		App->colliders.MoveObject(&player.player_rect, { 0, 4});
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
	}




	if (current_animation == &jump && player.player_speed.y < 12 && player.player_god_mode == false)player.player_speed.y += gravity;



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
	App->render->MoveCameraToPointInsideLimits({player.player_rect.x, player.player_rect.y});


	//App->render->DrawQuad({ player.player_position.x, player.player_position.y, App->map->data.tile_width, App->map->data.tile_height}, 255, 255, 255, 255);
	if (player.player_flip == false && player.player_speed.x < 0)player.player_flip = true;
	else if (player.player_flip == true && player.player_speed.x > 0)player.player_flip = false;
	App->render->Blit(player.player_spritesheet, player.player_rect.x, player.player_rect.y - current_frame.h, &current_frame, player.player_flip);
	App->render->DrawQuad({ player.player_rect.x, player.player_rect.y, player.player_rect.w, player.player_rect.h }, 255, 255, 255, 55);
	for (int i = 0; i < inputs_out; i++)input_out[i] = O_NONE;
	inputs_out = 0;
	input_in = I_NONE;

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
					player.player_flip = false;
					player.player_not_jumping = true;
					player.player_god_mode = false;
					player.player_tang_mode = false;
					inputs_out = 0;
					actual_state = S_IDLE;
				}
				isSpawn = isSpawn->next;
			}
		}
		objects_map=objects_map->next;
	}
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

			}
		}
	}
}