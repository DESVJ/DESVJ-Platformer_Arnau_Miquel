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
			int num = CheckCommand();
			if (num == 0) {
				LOG("Command not found");
			}
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

int Console::CheckCommand() {
	int num_command;
	const char* command = console_input->GetLabel().GetString();
	if (!strcmp(command, "list"))
		num_command = 1;
	else if (!strcmp(command, "god_mode") || !strcmp(command, "god mode") || !strcmp(command, "godmode"))
		num_command = 2;
	else if (!strcmp(command, "quit"))
		num_command = 3;
	else if (!strcmp(command, "FPS"))
		num_command = 4;
	else if (!strcmp(command, "map"))
		num_command = 5;
	else 
		num_command = 0;

	return num_command;
}