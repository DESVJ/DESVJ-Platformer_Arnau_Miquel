#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	bool mouse = false;
	int count = 0;
	if (App->input->GetMouseButtonDown(1) == KEY_DOWN || App->input->GetMouseButtonDown(1) == KEY_REPEAT) {
		for (int i = UIs.count() - 1; i >= 0 && mouse == false; i--) {
			mouse = UIs.At(i)->data->CheckMouse();
			if (mouse == true)count = i;
		}
	}
	if (mouse == true) {
		UIs.At(count)->data->Move();
	}
	for (int i = 0; i < UIs.count(); i++) {
		UIs.At(i)->data->PreUpdate();
	}
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (int i = 0; i < UIs.count(); i++) {
		UIs.At(i)->data->PostUpdate();
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	for (int i = UIs.count() - 1; i >= 0; i--)
	{
		UIs.At(i)->data->CleanUp();
		UIs.del(UIs.At(i));
	}
	UIs.clear();
	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

UI* j1Gui::CreateUIElement(Type type, UI* p, SDL_Rect r, SDL_Rect sprite, p2SString str, SDL_Rect sprite2, SDL_Rect sprite3)
{
	UI* ui;
	switch (type)
	{
	case Type::BUTTON:
		ui = new ButtonUI(Type::BUTTON, p, r, sprite, sprite2, sprite3, true);
		break;
	case Type::IMAGE:
		ui = new ImageUI(Type::IMAGE, p, r, sprite, true);
		break;
	case Type::WINDOW:
		ui = new WindowUI(Type::WINDOW, p, r, sprite, true);
		break;
	case Type::TEXT:
		ui = new TextUI(Type::TEXT, p, r, str, false);
		break;
	}
	UIs.add(ui);
	return ui;
}

bool j1Gui::DeleteUIElement(UI* ui) {
	int n = UIs.find(ui);
	if (n == -1)return false;
	else
	{
		UIs.At(n)->data->CleanUp();
		UIs.del(UIs.At(n));
		return true;
	}
}

UI::UI(Type type, SDL_Rect r, UI* p, bool d) {
	name.create("UI");
	drageable = d;
	screen_rect = { r.x,r.y,r.w,r.h };
	parent = p;
	if (parent != nullptr) {
		local_rect = { screen_rect.x - parent->screen_rect.x, screen_rect.y - parent->screen_rect.y, screen_rect.w, screen_rect.h };
	}
	else {
		local_rect = screen_rect;
	}
	mask_rect = screen_rect;
}

bool UI::PreUpdate() {
	UI* ui = this;

	screen_rect.x = local_rect.x;
	screen_rect.y = local_rect.y;
	while (ui->parent != nullptr) {
		screen_rect.x += ui->parent->local_rect.x;
		screen_rect.y += ui->parent->local_rect.y;
		ui = ui->parent;
	}

	uint win_x, win_y;
	App->win->GetWindowSize(win_x, win_y);
	mask_rect = screen_rect;

	if (parent != nullptr) {
		if (mask_rect.x < parent->mask_rect.x) {
			mask_rect.x += parent->mask_rect.x - mask_rect.x;
			mask_rect.w -= parent->mask_rect.x - mask_rect.x;
		}
		else if (mask_rect.x + mask_rect.w > parent->mask_rect.x + parent->mask_rect.w) {
			mask_rect.w -= mask_rect.x + mask_rect.w - parent->mask_rect.x - parent->mask_rect.w;
		}
		if (mask_rect.y < parent->mask_rect.y) {
			mask_rect.y += parent->mask_rect.y - mask_rect.y;
			mask_rect.h -= parent->mask_rect.y - mask_rect.y;
		}
		else if (mask_rect.y + mask_rect.h > parent->mask_rect.y + parent->mask_rect.h) {
			mask_rect.h -= mask_rect.y + mask_rect.h - parent->mask_rect.y - parent->mask_rect.h;
		}
	}
	else {
		if (mask_rect.x < 0) {
			mask_rect.w -= mask_rect.x;
			mask_rect.x = 0;
		}
		else if (mask_rect.x + mask_rect.w > win_x) {
			mask_rect.w -= mask_rect.x + mask_rect.w - win_x;
		}
		if (mask_rect.y < 0) {
			mask_rect.h -= mask_rect.y;
			mask_rect.y = 0;
		}
		else if (mask_rect.y + mask_rect.h > win_y) {
			mask_rect.h -= mask_rect.y + mask_rect.h - win_y;
		}
	}
	return true;
}

SDL_Rect UI::GetScreenRect() {
	return screen_rect;
}
SDL_Rect UI::GetLocalRect() {
	return local_rect;
}
iPoint UI::GetScreenPos() {
	return { screen_rect.x,screen_rect.y };
}
iPoint UI::GetLocalPos() {
	return { local_rect.x,local_rect.y };
}
void UI::SetLocalPos(iPoint pos) {
	iPoint r = { -local_rect.x + pos.x,-local_rect.y + pos.y };
	local_rect.x = pos.x;
	local_rect.y = pos.y;
	screen_rect.x += r.x;
	screen_rect.y += r.y;
}

bool UI::CheckMouse() {
	if (drageable == true) {
		int x, y;
		App->input->GetMousePosition(x, y);
		if (x >= screen_rect.x && x <= screen_rect.x + screen_rect.w && y >= screen_rect.y && y <= screen_rect.y + screen_rect.h)
			return true;
	}
	return false;
}

bool UI::Move() {
	int x, y;
	App->input->GetMouseMotion(x, y);
	local_rect.x += x;
	local_rect.y += y;
	return true;
}

SDL_Rect UI::Check_Printable_Rect(SDL_Rect sprite, iPoint& dif_sprite) {
	if (mask_rect.x > screen_rect.x) {
		dif_sprite.x = mask_rect.x - screen_rect.x;
		sprite.x += dif_sprite.x;
		sprite.w -= dif_sprite.x;
	}
	else if (mask_rect.w < screen_rect.w) {
		sprite.w -= screen_rect.w - mask_rect.w;
	}
	if (mask_rect.y > screen_rect.y) {
		dif_sprite.y = mask_rect.y - screen_rect.y;
		sprite.y += dif_sprite.y;
		sprite.h -= dif_sprite.y;
	}
	else if (mask_rect.h < screen_rect.h) {
		sprite.h -= screen_rect.h - mask_rect.h;
	}
	return sprite;
}

ImageUI::ImageUI(Type type, UI* p, SDL_Rect r, SDL_Rect sprite, bool d) :UI(type, r, p, d) {
	name.create("ImageUI");
	sprite1 = sprite;
}

bool ImageUI::PostUpdate() {
	iPoint dif_sprite = { 0,0 };
	SDL_Rect sprite = UI::Check_Printable_Rect(sprite1, dif_sprite);
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), GetScreenPos().x + dif_sprite.x, GetScreenPos().y + dif_sprite.y, &sprite);
	return true;
}

WindowUI::WindowUI(Type type, UI* p, SDL_Rect r, SDL_Rect sprite, bool d) :UI(type, r, p, d) {
	name.create("WindowUI");
	sprite1 = sprite;
}

bool WindowUI::PostUpdate() {
	iPoint dif_sprite = { 0,0 };
	SDL_Rect sprite = UI::Check_Printable_Rect(sprite1, dif_sprite);
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), GetScreenPos().x + dif_sprite.x, GetScreenPos().y + dif_sprite.y, &sprite);
	return true;
}

TextUI::TextUI(Type type, UI* p, SDL_Rect r, p2SString str, bool d) :UI(type, r, p, d) {
	name.create("TextUI");
	stri = str;
	int w, h;
}

bool TextUI::PostUpdate() {
	SDL_Rect rect = { 0,0,0,0 };
	iPoint dif_sprite = { 0,0 };
	SDL_QueryTexture(App->font->Print(stri.GetString()), NULL, NULL, &rect.w, &rect.h);
	SDL_Rect sprite = UI::Check_Printable_Rect(rect, dif_sprite);
	App->render->Blit(App->font->Print(stri.GetString()), GetScreenPos().x + dif_sprite.x, GetScreenPos().y + dif_sprite.y, &sprite);
	return true;
}

ButtonUI::ButtonUI(Type type, UI* p, SDL_Rect r, SDL_Rect sprite, SDL_Rect spriten2, SDL_Rect spriten3, bool d) :UI(type, r, p, d) {
	name.create("ButtonUI");
	sprite1 = sprite;
	sprite2 = spriten2;
	sprite3 = spriten3;
	over = false;
	pushed = false;
}

bool ButtonUI::PostUpdate() {
	SDL_Rect sprite;
	iPoint dif_sprite = { 0,0 };
	if (pushed == true) {
		sprite = UI::Check_Printable_Rect(sprite2, dif_sprite);
	}
	else if (over == true) {
		sprite = UI::Check_Printable_Rect(sprite1, dif_sprite);
	}
	else {
		sprite = UI::Check_Printable_Rect(sprite3, dif_sprite);
	}
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), GetScreenPos().x + dif_sprite.x, GetScreenPos().y + dif_sprite.y, &sprite);
	return true;
}

bool ButtonUI::PreUpdate() {
	int x, y;
	App->input->GetMousePosition(x, y);
	if (x >= GetScreenPos().x && x <= GetScreenPos().x + GetScreenRect().w && y >= GetScreenPos().y && y <= GetScreenPos().y + GetScreenRect().h)
		over = true;
	else over = false;
	bool button = false;
	if (App->input->GetMouseButtonDown(1) == KEY_DOWN || App->input->GetMouseButtonDown(1) == KEY_REPEAT)
		button = true;
	if (over == true && button == true)
		pushed = true;
	else pushed = false;
	UI::PreUpdate();
	return true;
}
