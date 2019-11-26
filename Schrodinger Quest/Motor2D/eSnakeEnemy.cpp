#include"eSnakeEnemy.h"


// Called before render is available
bool eSnakeEnemy::Awake(pugi::xml_node& config)
{
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
	App->render->Blit(spritesheet, 50, 50);
	return true;
}

bool eSnakeEnemy::CleanUp()
{

	eCreature::CleanUp();



	return true;
}


