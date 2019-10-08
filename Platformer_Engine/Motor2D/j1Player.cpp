#include"j1Player.h"
#include"j1App.h"
#include"j1Render.h"
#include"j1Map.h"

j1Player::j1Player() : j1Module()
{
	name.create("input");

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

	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{

	App->render->DrawQuad({ App->map->data.tile_width, 50, App->map->data.tile_width, App->map->data.tile_height}, 255, 255, 255, 255);

	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{

	return true;
}
