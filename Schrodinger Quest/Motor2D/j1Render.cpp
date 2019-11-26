#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include"j1Input.h"
#include"j1Player.h"
#include "EntityManager.h"
#include "brofiler/Brofiler.h"

#define VSYNC true

j1Render::j1Render() : j1Module()
{
	name.create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render()
{}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool j1Render::PostUpdate()
{BROFILER_CATEGORY("Render PostUpdate", Profiler::Color::Blue)
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void j1Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void j1Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, bool flip, fPoint speed, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;
	SDL_RendererFlip flip_render = SDL_FLIP_NONE;
	uint scale = App->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed.x) + x * scale;
	rect.y = (int)(camera.y * speed.y) + y * scale;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}
	if (flip == true)flip_render = SDL_FLIP_HORIZONTAL;
	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip_render/*SDL_FLIP_NONE*/) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if (use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for (uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void  j1Render::MoveCameraToPointInsideLimits(p2Point<int> point)
{

	//Convert point from world space to camera space
	int x = -(point.x * (int)App->win->GetScale()) + (App->win->width / 2);
	int y = -(point.y * (int)App->win->GetScale()) + (App->win->height / 2);

	//Move follow rect to center of camera
	followMinRect.x = (-camera.x + ((int)App->win->width / 2)) / (int)App->win->GetScale() - (followMinRect.w / 2.f);
	followMinRect.y = (-camera.y + ((int)App->win->height / 2)) / (int)App->win->GetScale() - (followMinRect.h * 0.6f);
	

	//Left X mov
	if (camera.x >= limitNegX)
	{
		camera.x = limitNegX;
	}
	else if (point.x <= followMinRect.x)
	{
		camera.x = x - ((followMinRect.w / 2) * (int)App->win->GetScale());
	}

	//Right X mov
	if (camera.x <= -limitPosX)
	{
		camera.x = -limitPosX;
	}
	else if (point.x >= followMinRect.x + followMinRect.w)
	{
		camera.x = x + ((followMinRect.w / 2) * (int)App->win->GetScale());
	}


	//Up Y mov
	if (camera.y >= limitPosY)
	{
		camera.y = limitPosY;
	}
	else if (point.y + App->entity_manager->Player->player.minPlayerHeight <= followMinRect.y)
	{
		if(y + (App->entity_manager->Player->player.minPlayerHeight / 2) * (int)App->win->GetScale() <= 0)
			camera.y = y + (App->entity_manager->Player->player.minPlayerHeight / 2) * (int)App->win->GetScale();
	}

	//Down Y mov
	if (camera.y <= -limitNegY)
	{
		camera.y = -limitNegY;
	}
	else if (point.y >= followMinRect.y + followMinRect.h)
	{
		camera.y = y + -App->entity_manager->Player->player.minPlayerHeight * (int)App->win->GetScale();
	}


	if(App->input->is_Debug_Mode)
		App->render->DrawQuad(followMinRect, 255, 210, 78, 50);

}

void j1Render::SetMapLimitsWithTMX()
{

	p2List_item<MapObjectGroup*>* itemA;
	itemA = App->map->data.object_layers.start;
	while (itemA != NULL)
	{
		//For each prop in that layer
		p2List_item<object_property*>* isLimit;
		isLimit = itemA->data->properties.start;
		while (isLimit != NULL)
		{
			//If the layer is a map limit
			if (isLimit->data->name == "isMapLimits" && isLimit->data->prop_value.value_bool == true)
			{
				//Set map limit on X
				for (unsigned int i = 0; i < itemA->data->objects.count(); i++)
				{
					if (itemA->data->objects[i]->properties[0]->name == "LimitX") 
					{
						limitPosX = itemA->data->objects[i]->rect.x * (int)App->win->GetScale() - camera.w;
					}
					else if(itemA->data->objects[i]->properties[0]->name == "LimitY")
					{
						limitNegY = itemA->data->objects[i]->rect.y * (int)App->win->GetScale() - camera.h;
					}
				}
			}
			isLimit = isLimit->next;
		}
		itemA = itemA->next;
	}
}