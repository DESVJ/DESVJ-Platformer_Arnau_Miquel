#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Gui.h"
#include "j1Fonts.h"

struct SDL_Texture;

enum class MenuType {
	MAINMENU,
	SETTINGS,
	CREDITS,
	PLAYERHUD
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	//// Called before all Updates
	//bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Load map
	void Load_Map_By_Name(const char*);
	void OnClick(UI* element);

	//UI creation
	void CreateMenu(MenuType type);


	bool isMainMenu;
	SDL_Texture* main_menu_background;

	WindowUI* menu_window;
	ButtonUI* menu_buttons[3];
	ImageUI* menu_text;



};

#endif // __j1SCENE_H__