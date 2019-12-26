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
	isMainMenu = true;
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

	main_menu_background = App->tex->Load("maps/main_menu.png");
	App->audio->PlayMusic("audio/music/Desert_Theme.ogg");

	// TODO 3: Create the banner (rect {485, 829, 328, 103}) as a UI element
	//App->gui->CreateUIElement(Type::IMAGE, { 485,829,328,103 });
	// TODO 4: Create the text "Hello World" as a UI element

	int w, h;
	App->font->CalcSize("Schrodinger Quest", w, h);
	//Parents are not working
	UI* window = App->gui->CreateUIElement(Type::WINDOW, nullptr, { ((int)App->win->width / 2) - 550 / 2, 15, 550, ((int)App->win->height - 30) });

	//Game title
	App->gui->CreateUIElement(Type::TEXT, window, { ((int)App->win->width / 2) - w * 3 / 2, 100, w * 3, h * 3 }, { 0, 0, 229, 69 }, "Schrodinger Quest");

	//Main buttons
	App->gui->CreateUIElement(Type::BUTTON, window, { ((int)App->win->width / 2) - 229 / 2, 200, 229, 69 }, { 114, 0, 114, 35 }, "PLAY", { 0, 0, 114, 35 }, { 0, 35, 114, 35 }, this);
	App->gui->CreateUIElement(Type::BUTTON, window, { ((int)App->win->width / 2) - 229 / 2, 300, 229, 69 }, { 114, 0, 114, 35 }, "CONTINUE", { 0, 0, 114, 35 }, { 0, 35, 114, 35 }, this);
	App->gui->CreateUIElement(Type::BUTTON, window, { ((int)App->win->width / 2) - 229 / 2, 400, 229, 69 }, { 114, 0, 114, 35 }, "SETTINGS", { 0, 0, 114, 35 }, { 0, 35, 114, 35 }, this);
	App->gui->CreateUIElement(Type::BUTTON, window, { ((int)App->win->width / 2) - 229 / 2, 500, 229, 69 }, { 114, 0, 114, 35 }, "CREDITS", { 0, 0, 114, 35 }, { 0, 35, 114, 35 }, this);
	App->gui->CreateUIElement(Type::BUTTON, window, { ((int)App->win->width / 2) - 229 / 2, 600, 229, 69 }, { 114, 0, 114, 35 }, "EXIT", { 0, 0, 114, 35 }, { 0, 35, 114, 35 }, this);

	//Github button
	App->gui->CreateUIElement(Type::BUTTON, nullptr, { 10, (int)App->win->height - 80, 70, 70 }, { 0, 165, 61, 60 }, "GITHUB",  { 61, 105, 61, 60 }, { 0, 105, 61, 60 }, this);


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	//if (!isMainMenu) 
	//{
	//	// debug pathfing ------------------
	//	static iPoint origin;
	//	static bool origin_selected = false;

	//	int x, y;
	//	App->input->GetMousePosition(x, y);
	//	App->map->Translate_Coord(&x, &y);

	//	iPoint p = { x, y };

	//	//PathFinding with	E
	//	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	//	{
	//		if (origin_selected == true)
	//		{
	//			App->map->WorldToMap(&origin.x, &origin.y);

	//			p.x = App->entity_manager->Player->collider->collider_rect.x;
	//			p.y = App->entity_manager->Player->collider->collider_rect.y + (App->entity_manager->Player->collider->collider_rect.h / 2);

	//			App->map->WorldToMap(&p.x, &p.y);
	//			App->pathfinding->CreatePath(origin, p);
	//			origin_selected = false;
	//		}
	//		else
	//		{
	//			origin = p;
	//			origin_selected = true;
	//		}
	//	}
	//}

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
	
	//Debug UI
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
		App->gui->ChangeDebug();

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

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		App->gui->ChangeFocus();

	//Draw map
	if (!isMainMenu) 
	{
		App->map->Draw();
	}
	else
	{
		App->render->BlitBackgroud(main_menu_background);
	}

	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	if (main_menu_background) 
	{
		App->tex->UnLoad(main_menu_background);
	}


	return true;
}

void j1Scene::OnClick(UI* element) 
{

	const char* a = element->name.GetString();
	switch (element->type)
	{

	case Type::BUTTON:

		if (element->name == (p2SString)"PLAY")
		{
			Load_Map_By_Name(App->map->GetSourceFromID(App->map->map_id).GetString());
		}
		else if (element->name == (p2SString)"CONTINUE")
		{
			//TODO: Only load if there is a saved file
			//Load game does not work
			//App->LoadGame();

		}		
		else if (element->name == (p2SString)"EXIT")
		{
			//Exit game
			exitGame = true;
		}
		else if(element->name == (p2SString)"GITHUB")
		{
			ShellExecuteA(NULL, "open", "https://github.com/DESVJ/Schrodinger_Quest", NULL, NULL, SW_SHOWNORMAL);
		}
		break;


	default:
		break;
	}


}

//Load map by name
void j1Scene::Load_Map_By_Name(const char* name)
{

	if (isMainMenu) {
		App->gui->ClearUI();
		isMainMenu = false;
	}




	App->map->active = true;
	App->entity_manager->active = true;

	App->colliders->ClearColliders();
	App->map->CleanUp();

	App->map->Load(name);
	App->pathfinding->UpdatePathFindingMap();

	App->colliders->LoadColliders();
	App->render->SetMapLimitsWithTMX();
	App->entity_manager->Player->Start_F3();
}