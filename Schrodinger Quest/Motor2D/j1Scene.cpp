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
	transitionState = 0;
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

	CreateMenu(MenuType::MAINMENU);


	uint w, h;
	App->win->GetWindowSize(w, h);
	transition = { 0, 0, (int)w, 0 };

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

bool j1Scene::PostUpdate()
{
	if (transitionState == 1)
	{
		if (transition.h + 200 * App->GetDT() <= App->win->height / (int)App->win->GetScale())
		{
			transition.h += 200 * App->GetDT();
		}
		else
		{
			transition.h = App->win->height * 2;
			Load_Map_By_Name(App->map->GetSourceFromID(App->map->map_id).GetString());
			CreateMenu(MenuType::PLAYERHUD);
			transitionState = 2;
		}



		App->render->DrawQuad(transition, 0, 0, 0);
		//LOG("%i, %i, %i, %i", transition.x, transition.y, transition.w, transition.h);
	}
	if (transitionState == 2) 
	{
		if (transition.h - 400 * App->GetDT() >= 0)
		{
			transition.h -= 400 * App->GetDT();
		}
		else
		{
			transition.h = 0;
			transitionState = 0;
		}



		App->render->DrawQuad(transition, 0, 0, 0, 255, true, false);
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
			transitionState = 1;
		}
		else if (element->name == (p2SString)"CONTINUE")
		{
			//TODO: Only load if there is a saved file
			//Load game does not work
			//App->LoadGame();
		}
		else if (element->name == (p2SString)"SETTINGS")
		{
			CreateMenu(MenuType::SETTINGS);
		}
		else if (element->name == (p2SString)"RETURN")
		{
			CreateMenu(MenuType::MAINMENU);
		}
		else if (element->name == (p2SString)"CREDITS")
		{
			CreateMenu(MenuType::CREDITS);
		}
		else if (element->name == (p2SString)"EXIT")
		{
			//Exit game
			exitGame = true;
		}
		else if (element->name == (p2SString)"GITHUB")
		{
			ShellExecuteA(NULL, "open", "https://github.com/DESVJ/Schrodinger_Quest", NULL, NULL, SW_SHOWNORMAL);
		}
		break;


	default:
		break;
	}


}

void j1Scene::CreateMenu(MenuType type) 
{
	App->gui->ClearUI();
	UI* window = nullptr;
	UI* playButton = nullptr;
	iPoint midPoint = App->win->GetScreenMidPoint();

	switch (type)
	{
	case MenuType::MAINMENU:

		window = App->gui->CreateUIElement(Type::WINDOW, nullptr, { midPoint.x - 550 / 2, 15, 550, ((int)App->win->height - 30) });
		//Game title
		App->gui->CreateUIElement(Type::IMAGE, window, { midPoint.x - 500 / 2, 60, 500, 154}, { 124, 110, 319, 104 }, "Schrodinger Quest");
		
		//Main buttons
		playButton = App->gui->CreateUIElement(Type::BUTTON, window, { midPoint.x - 229 / 2, 270, 229, 88 }, { 201, 225, 201, 88 }, "PLAY", { 402, 225, 201, 88 }, { 0, 225, 201, 88 }, false, { 0,0,0,0 }, this);
		App->gui->CreateUIElement(Type::IMAGE, playButton, { midPoint.x - 175 / 2, 290, 175, 40 }, { 114, 35, 306, 35 });

		playButton = App->gui->CreateUIElement(Type::BUTTON, window, { midPoint.x - 229 / 2, 370, 229, 88 }, { 201, 225, 201, 88 }, "CONTINUE", { 402, 225, 201, 88 }, { 0, 225, 201, 88 }, false, { 0,0,0,0 }, this);
		App->gui->CreateUIElement(Type::IMAGE, playButton, { midPoint.x - 175 / 2, 390, 175, 40 }, { 114, 72, 291, 35 });

		playButton = App->gui->CreateUIElement(Type::BUTTON, window, { midPoint.x - 229 / 2, 470, 229, 88 }, { 201, 225, 201, 88 }, "CREDITS", { 402, 225, 201, 88 }, { 0, 225, 201, 88 }, false, { 0,0,0,0 }, this);
		App->gui->CreateUIElement(Type::IMAGE, playButton, { midPoint.x - 175 / 2, 490, 175, 40 }, { 420, 35, 238, 35 });

		App->gui->CreateUIElement(Type::BUTTON, window, { midPoint.x - 220 / 2, 570, 80, 80 }, { 432, 545, 108, 116 }, "SETTINGS", { 540, 545, 108, 116 }, { 324, 545, 108, 116 }, false, { 0,0,0,0 }, this);
		App->gui->CreateUIElement(Type::BUTTON, window, { midPoint.x + 30, 570, 80, 80 }, { 432, 893, 108, 116 }, "EXIT", { 540, 893, 108, 116 }, { 324, 893, 108, 116 }, false, { 0,0,0,0 }, this);
		//Github button
		App->gui->CreateUIElement(Type::BUTTON, nullptr, { 10, (int)App->win->height - 60, 50, 50 }, { 108, 313, 108, 116 }, "GITHUB", { 216, 314, 108, 116 }, { 0, 313, 108, 116 }, false, { 0,0,0,0 }, this);
		
		break;
	case MenuType::SETTINGS:
		window = App->gui->CreateUIElement(Type::WINDOW, nullptr, { midPoint.x - 550 / 2, 25, 550, ((int)App->win->height - 50) });
		App->gui->CreateUIElement(Type::BUTTON, window, { midPoint.x + 30, 470, 60, 60 }, { 432, 893, 108, 116 }, "RETURN", { 540, 893, 108, 116 }, { 324, 893, 108, 116 }, false, { 0,0,0,0 }, this);

		break;
	case MenuType::CREDITS:
		window = App->gui->CreateUIElement(Type::WINDOW, nullptr, { midPoint.x - 550 / 2, 25, 550, ((int)App->win->height - 50) });
		App->gui->CreateUIElement(Type::BUTTON, window, { midPoint.x + 30, 470, 60, 60 }, { 432, 893, 108, 116 }, "RETURN", { 540, 893, 108, 116 }, { 324, 893, 108, 116 }, false, { 0,0,0,0 }, this);

		break;
	case MenuType::PLAYERHUD:
		window = App->gui->CreateUIElement(Type::WINDOW, nullptr, { 10, 10, 250, 60 });

		App->gui->CreateUIElement(Type::IMAGE, window, { 40,  20, 35, 38 }, {228, 0, 10, 12});
		App->gui->CreateUIElement(Type::IMAGE, window, { 100, 20, 35, 38 }, {228, 0, 10, 12});
		App->gui->CreateUIElement(Type::IMAGE, window, { 160, 20, 35, 38 }, {228, 0, 10, 12});

		App->gui->CreateUIElement(Type::TEXT, nullptr, { midPoint.x - 250 / 2, 10, 250, 60 }, {0, 0, 0, 0}, "SCORE: 0000");

		//App->gui->CreateUIElement(Type::BUTTON, nullptr, { (int)App->win->width - 50, 10, 40, 40 }, { 433, 777, 109, 116 }, "PAUSE", { 542, 777, 108, 116 } , { 325, 777, 108, 116 }, this);
		break;
	}
	App->gui->ReturnConsole();
}

//Load map by name
void j1Scene::Load_Map_By_Name(const char* name)
{

	if (isMainMenu) {
		App->gui->ClearUI();
		App->gui->ReturnConsole();
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