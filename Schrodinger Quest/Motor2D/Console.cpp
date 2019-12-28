#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Console.h"
#include "j1Gui.h"
#include "j1Input.h"

Console::Console() {
	name.create("console");
	console_active = false;
}


bool Console::Awake(pugi::xml_node& node) {
	background_pos = { node.child("background").attribute("position_x").as_int(),node.child("background").attribute("position_y").as_int(),node.child("background").attribute("position_w").as_int(),
	node.child("background").attribute("position_h").as_int() };
	background_red = node.child("background").attribute("red").as_int();
	background_green = node.child("background").attribute("green").as_int();
	background_blue = node.child("background").attribute("blue").as_int();
	background_alpha = node.child("background").attribute("alpha").as_int();
	return true;
}

bool Console::PreUpdate() {
	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN) 
	{
		console_active = !console_active;
		if (console_active == true) {
			ActivateConsole();
		}
		else {
			App->gui->DeleteUIElement(console_background);
		}
	}
	return true;
}

void Console::ActivateConsole() {
	console_background = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, nullptr, background_pos, "", background_red, background_green, background_blue, background_alpha);
}