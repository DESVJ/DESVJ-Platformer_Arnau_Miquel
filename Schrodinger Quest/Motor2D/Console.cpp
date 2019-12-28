#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Console.h"
#include "j1Player.h"
#include "j1Gui.h"
#include "EntityManager.h"
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

	input_pos = { node.child("input").attribute("position_x").as_int(),node.child("input").attribute("position_y").as_int(),node.child("input").attribute("position_w").as_int(),
	node.child("input").attribute("position_h").as_int() };
	input_red = node.child("input").attribute("red").as_int();
	input_green = node.child("input").attribute("green").as_int();
	input_blue = node.child("input").attribute("blue").as_int();
	input_alpha = node.child("input").attribute("alpha").as_int();

	label = "";
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
			App->gui->DeleteUIElement(console_input);
		}
	}
	if (console_active == true) {
		console_input->focus = true;
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			CheckCommand();
			ExecuteCommand();
			console_input->SetLabel("");
		}
		label = console_input->GetLabel();
	}
	return true;
}

void Console::ActivateConsole() {
	console_background = (ImageUI*)App->gui->CreateUIElement(Type::IMAGE, nullptr, background_pos, "", background_red, background_green, background_blue, background_alpha);
	console_input = (TextInputUI*)App->gui->CreateUIElement(Type::INPUT, nullptr, input_pos, "", input_red, input_green, input_blue, input_alpha);
	console_input->SetLabel(label.GetString());
}

void Console::CheckCommand() {
	const char* command_text = console_input->GetLabel().GetString();
	if (!strcmp(command_text, "list"))
		command = commands::list;
	else if (!strcmp(command_text, "god_mode") || !strcmp(command_text, "god mode") || !strcmp(command_text, "godmode"))
		command = commands::god_mode;
	else if (!strcmp(command_text, "quit"))
		command = commands::quit;
	else if (!strcmp(command_text, "FPS") || !strcmp(command_text, "Fps") || !strcmp(command_text, "fps"))
		command = commands::FPS;
	else if (!strcmp(command_text, "map"))
		command = commands::map;
	else 
		command = commands::none;
}

void Console::ExecuteCommand() {
	switch (command) {
	case commands::list:
		LOG("Commands available: list, god_mode, quit, FPS, map");
		break;

	case commands::god_mode:
		if (App->entity_manager->Player != NULL) {
			App->entity_manager->Player->player.player_god_mode = !App->entity_manager->Player->player.player_god_mode;
			App->entity_manager->Player->player.player_not_jumping = true;
			App->entity_manager->Player->player.spacebar_pushed = false;
		}
		else {
			LOG("You cannot go to god_mode because you are not playing");
		}
		break;

	case commands::quit:
		exitGame = true;
		break;

	case commands::FPS:
		break;

	case commands::map:
		break;

	case commands::none:
		LOG("Command not found");
		break;
	}
}