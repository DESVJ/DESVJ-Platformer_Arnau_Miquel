#include"j1Player.h"
#include"j1App.h"
#include"j1Render.h"
#include"j1Map.h"
#include"j1Input.h"
#include"j1Textures.h"
#include "Animation.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");

	//Idle animation
	idle.PushBack({ 6,11,16,20 });
	idle.PushBack({ 37,10,19,21 });
	idle.PushBack({ 69,10,19,21 });
	idle.PushBack({ 101,10,19,21 });
	idle.PushBack({ 134,11,16,20 });
	idle.PushBack({ 166,11,16,20 });
	idle.PushBack({ 198,11,16,20 });
	idle.PushBack({ 230,11,16,20 });
	idle.PushBack({ 262,11,16,20 });
	idle.PushBack({ 294,11,16,20 });
	idle.PushBack({ 326,11,16,20 });
	idle.speed = 0.2f;

	//Run animation
	run.PushBack({ 5,42,18,19 });
	run.PushBack({ 37,41,18,18 });
	run.PushBack({ 69,42,18,19 });
	run.PushBack({ 103,43,14,20 });
	run.PushBack({ 136,42,12,20 });
	run.PushBack({ 168,41,12,20 });
	run.PushBack({ 200,42,12,20 });
	run.PushBack({ 231,43,14,20 });
	run.speed = 0.2f;

	//Jump animation
	jump.PushBack({ 6,171,16,20 });
	jump.PushBack({ 36,172,19,19 });
	jump.PushBack({ 69,169,18,20 });
	jump.PushBack({ 102,167,18,19 });
	jump.PushBack({ 133,167,18,20 });
	jump.PushBack({ 164,172,19,19 });
	jump.speed = 0.1f;

}

// Destructor
j1Player::~j1Player()
{

}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& config)
{
	
	return true;
}

// Called before the first frame
bool j1Player::Start()
{
	player.player_position.x = App->map->data.tile_width * 2;
	player.player_position.y = App->map->data.tile_width * 8;
	player.player_spritesheet = App->tex->Load("textures/Adventurer Sprite Sheet v1.1.png");
	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{

	Animation* current_animation = &idle;
	SDL_Rect current_frame = current_animation->GetCurrentFrame();


		//App->render->DrawQuad({ player.player_position.x, player.player_position.y, App->map->data.tile_width, App->map->data.tile_height}, 255, 255, 255, 255);
		App->render->Blit(player.player_spritesheet, player.player_position.x, player.player_position.y, &current_frame);
		App->render->DrawQuad({ player.player_position.x, player.player_position.y, App->map->data.tile_width, App->map->data.tile_height }, 255, 255, 255, 55);


	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(player.player_spritesheet);
	return true;
}
