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
	App->map->Load("map_1_final.tmx");
	App->colliders.LoadColliders();
	App->render->SetMapLimitsWithTMX();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->MoveCameraInsideLimits(0, 3 * App->win->GetScale());

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->MoveCameraInsideLimits(0, (-3 * App->win->GetScale()));

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->MoveCameraInsideLimits(3 * App->win->GetScale(), 0);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->MoveCameraInsideLimits((-3 * App->win->GetScale()), 0);

	if (App->input->GetKey(SDL_SCANCODE_RIGHTBRACKET) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHTBRACKET) == KEY_REPEAT)
		App->audio->ChangeVolume(true);

	if (App->input->GetKey(SDL_SCANCODE_SLASH) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_SLASH) == KEY_REPEAT)
		App->audio->ChangeVolume(false);

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Load_Map_By_Name("map_1_final.tmx");

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		Load_Map_By_Name("map_2_final.tmx");

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->player->Start_F3();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->SaveGame();


	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

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


//Load map
void j1Scene::Load_Map_By_Name(const char* name)
{
	App->colliders.collider_list.clear();
	App->map->CleanUp();
	App->map->Load(name);
	App->colliders.LoadColliders();
	App->render->SetMapLimitsWithTMX();
	App->player->Start_F3();
}