#include"eEnemy.h"


bool eEnemy::Awake(pugi::xml_node& config) 
{

	pugi::xml_document	player_info_file;
	player_info_file.load_file(config.child("load_file").child_value());
	pugi::xml_node player_node = player_info_file.child("map");

	LoadAnimationFromTMX(&player_node, &idle, "idle");
	LoadAnimationFromTMX(&player_node, &move, "move");
	LoadAnimationFromTMX(&player_node, &death, "death");

	detection_range = config.child("detection_range").attribute("value").as_int();

	collision_rect.w = idle.GetCurrentFrameWithoutAnim().w;
	collision_rect.h = -idle.GetCurrentFrameWithoutAnim().h;

	if (!spritesheet)
	{
		//Remove hardcode when tmx of enemy is created
		spritesheet = App->tex->Load(config.child("texture_source").child_value());
	}

	LOG("%i, %i", collision_rect.w, collision_rect.h);
	return true;
}