#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "EntityManager.h"
#include "j1Pathfinding.h"


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//Load map 1 && collisions on game start
	App->map->Load(App->map->GetSourceFromID(App->map->map_id).GetString());
	App->colliders->LoadColliders();
	App->render->SetMapLimitsWithTMX();

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	App->map->Translate_Coord(&x, &y);

	iPoint p = { x, y };
	
	//PathFinding with	E
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->map->WorldToMap(&origin.x, &origin.y);

			p.x = App->entity_manager->Player->collision_rect.x;
			p.y = App->entity_manager->Player->collision_rect.y + (App->entity_manager->Player->collision_rect.h / 2);

			App->map->WorldToMap(&p.x, &p.y);
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	//Volume change +
	if (App->input->GetKey(SDL_SCANCODE_RIGHTBRACKET) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHTBRACKET) == KEY_REPEAT)
		App->audio->ChangeVolume(true);

	//Volume change -
	if (App->input->GetKey(SDL_SCANCODE_SLASH) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SLASH) == KEY_REPEAT)
		App->audio->ChangeVolume(false);

	//Start from the first level
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) 
	{
		App->map->map_id = 1;
		Load_Map_By_Name(App->map->GetSourceFromID(App->map->map_id).GetString());
	}

	//Start from the second level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) 
	{
		App->map->map_id = 2;
		Load_Map_By_Name(App->map->GetSourceFromID(App->map->map_id).GetString());
	}

	//Start from the beginning of the current level
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->entity_manager->Player->Start_F3();

	//Save the current state
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	//Load the previous state (even across levels)
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();
	
	//Enable and disable 30fps cap
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) 
	{
		if (App->fps_limit == App->fps_min) 
		{
			App->fps_limit = App->fps_max;
		}
		else
		{
			App->fps_limit = App->fps_min;
		}
	}

	//Draw map
	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	App->map->Translate_Coord(&x, &y);
	iPoint p = { x, y };
	App->render->DrawQuad({ p.x, p.y , 16, 16}, 255, 0, 0, 100);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		int x = path->At(i)->x;
		int y = path->At(i)->y;
		App->map->Translate_Coord(&x, &y);
		iPoint pos = { x, y };
		App->render->DrawQuad({ pos.x, pos.y, 16, 16 }, 0, 255, 0, 100);
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	//Close game
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}


//Load map by name
void j1Scene::Load_Map_By_Name(const char* name)
{
	App->colliders->collider_list.clear();
	App->map->CleanUp();

	App->map->Load(name);
	App->colliders->LoadColliders();
	App->render->SetMapLimitsWithTMX();
	App->entity_manager->Player->Start_F3();
}