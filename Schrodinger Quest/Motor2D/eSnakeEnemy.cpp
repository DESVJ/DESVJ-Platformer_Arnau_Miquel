#include"eSnakeEnemy.h"
#include "EntityManager.h"


// Called before render is available
bool eSnakeEnemy::Awake(pugi::xml_node& config)
{

	//pugi::xml_document	player_info_file;
	////p2SString s = config.child("ground_enemy").child("snake").child("load_file").child_value();
	//player_info_file.load_file(config.child("ground_enemy").child("snake").child("load_file").child_value());
	//pugi::xml_node player_node = player_info_file.child("map");

	//LoadAnimationFromTMX(&player_node, &idle, "idle");

	return true;
}

bool eSnakeEnemy::Start()
{

	//TODO THIS IS HARDCODED, CANT USE IT ON AWAKE BECASUSE ITS CALLED DURING RUNTIME, NEEDS TO BE FIXED
	pugi::xml_document	player_info_file;
	player_info_file.load_file("textures/Enemy_Sprites/snake.tmx");
	pugi::xml_node player_node = player_info_file.child("map");

	LoadAnimationFromTMX(&player_node, &idle, "idle");


	if (!spritesheet)
	{
		//Remove hardcode when tmx of enemy is created
		spritesheet = App->tex->Load("textures/Enemy_Sprites/Cobra_Sprite_Sheet.png");
	}

	return true;
}

bool eSnakeEnemy::PreUpdate() 
{
	iPoint p;
	iPoint origin = { position_rect.x, position_rect.y -10 };

	p.x = App->entity_manager->Player->collision_rect.x;
	p.y = App->entity_manager->Player->collision_rect.y + (App->entity_manager->Player->collision_rect.h / 2);

	App->map->WorldToMap(&p.x, &p.y);
	App->map->WorldToMap(&origin.x, &origin.y);
	PathNumber = App->pathfinding->CreatePath(origin, p);

	return true;
}

// Called each loop iteration
bool eSnakeEnemy::Update(float dt)
{
	SDL_Rect rec = idle.GetCurrentFrame();

	App->render->Blit(spritesheet, position_rect.x, position_rect.y - rec.h, &rec, flip);
	return true;
}

bool eSnakeEnemy::CleanUp()
{

	eCreature::CleanUp();



	return true;
}


