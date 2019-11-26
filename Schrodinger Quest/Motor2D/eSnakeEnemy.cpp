#include"eSnakeEnemy.h"


// Called before render is available
bool eSnakeEnemy::Awake(pugi::xml_node& config)
{

	//pugi::xml_node animation_node = config.child("enemy_info").child("ground_enemy").child("snake").child("load_file").value();

	pugi::xml_document	player_info_file;
	//p2SString s = config.child("ground_enemy").child("snake").child("load_file").child_value();
	player_info_file.load_file(config.child("ground_enemy").child("snake").child("load_file").child_value());
	pugi::xml_node player_node = player_info_file.child("map");

	LoadAnimationFromTMX(&player_node, &idle, "idle");
	position_rect.x = 20;
	position_rect.y = 600;

	//LOG("%f", idle.speed);

	return true;
}

bool eSnakeEnemy::Start()
{
	if (!spritesheet)
	{
		//Remove hardcode when tmx of enemy is created
		spritesheet = App->tex->Load("textures/Enemy_Sprites/Cobra_Sprite_Sheet.png");
	}

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


