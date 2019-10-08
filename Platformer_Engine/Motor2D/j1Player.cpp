#include"j1Player.h"
#include"j1App.h"
#include"j1Render.h"
#include"j1Map.h"
#include"j1Input.h"
#include"j1Textures.h"

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

	return true;
}

// Called before the first frame
bool j1Player::Start()
{
	player.player_position.x = App->map->data.tile_width * 2;
	player.player_position.y = App->map->data.tile_width * 8;
	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{





	App->render->DrawQuad({ player.player_position.x, player.player_position.y, App->map->data.tile_width, App->map->data.tile_height}, 255, 255, 255, 255);

	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(player.player_spritesheet);
	return true;
}
