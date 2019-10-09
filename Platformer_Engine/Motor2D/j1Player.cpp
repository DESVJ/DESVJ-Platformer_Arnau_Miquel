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
	inputs_out = 0;
	actual_state = S_IDLE;

	player.player_rect.x = App->map->data.tile_width * 2;
	player.player_rect.y = App->map->data.tile_width * 8;
	player.player_rect.w = 0;
	player.player_rect.h = 0;
	player.player_flip = false;

	player.player_spritesheet = App->tex->Load("textures/Player_SpriteSheet.png");
	return true;
}

bool j1Player::PreUpdate() {
	CheckInputs(inputs_out, player.player_speed.y, actual_state, input_in, input_out);
	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	bool reset_animation = CheckState(inputs_out, actual_state, input_in, input_out);
	Animation* current_animation = ExecuteState(player.player_speed, actual_state, reset_animation);
	if (reset_animation == true)current_animation->Reset();
		SDL_Rect current_frame = current_animation->GetCurrentFrame();
	//player.player_rect.w = current_frame.w;
	//player.player_rect.h = -current_frame.h;
	//player.player_rect.x += player.player_speed.x;
	//player.player_rect.y += player.player_speed.y;
	//if (current_animation == &jump && player.player_speed.y < 12)player.player_speed.y += gravity;

	//This must be debug mode only
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		App->colliders.MoveObject(&player.player_rect, {2, 0});
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		App->colliders.MoveObject(&player.player_rect, { -2, 0 });
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		App->colliders.MoveObject(&player.player_rect, { 0, -2 });
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		App->colliders.MoveObject(&player.player_rect, { 0, 2 });

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
