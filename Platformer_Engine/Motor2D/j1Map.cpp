//Code by Miquel Suau && Arnau Falagueras

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include"j1Player.h"
#include"j1Window.h"
#include"j1Input.h"
#include "j1Audio.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	map_id = config.child("maps").child("map").attribute("id").as_int();
	return ret;
}


bool j1Map::Culling_Check(int x, int y, SDL_Rect rect, float speed)
{


	int scale = (int)App->win->GetScale();

	if ((x + rect.w) * scale / speed >= -App->render->camera.x + culling_offset && x * scale <= -App->render->camera.x * speed + App->win->width - culling_offset
		&& (y + rect.h) * scale  >= -App->render->camera.y + culling_offset && y * scale <= -App->render->camera.y + App->win->height - culling_offset)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	bool ret = false;
	int number_of_layers = data.layers.count();
	p2List_item<MapLayer*>* coord_layer = data.layers.start;
	p2List_item<MapLayer*>* tang_coord_layer = data.tang_layers.start;
	p2List_item<MapLayer*>* background_coord_layer = data.background_layers.start;

	p2List_item<TileSet*>* coord_tileset = data.tilesets.start;
	//for of every layer


	for (int layer_counter = 0; layer_counter < data.background_layers.count(); layer_counter++) {

		//for of every x in one layer
		for (int i = 0; i < background_coord_layer->data->height; i++) {

			//for of every y in one layer

			for (int j = 0; j < background_coord_layer->data->width; j++) {
				int n = background_coord_layer->data->Get(j, i);
				int gid = background_coord_layer->data->gid[n];
				if (gid != 0) {
					while (ret == false) {
						if (coord_tileset->next != NULL && coord_tileset->next->data->firstgid <= gid) coord_tileset = coord_tileset->next;
						else if (coord_tileset->prev != NULL && coord_tileset->data->firstgid > gid)coord_tileset = coord_tileset->prev;
						else ret = true;
					}
					ret = false;
					SDL_Rect rect = coord_tileset->data->GetRect(background_coord_layer->data->gid[n]);
					int x = j;
					int y = i;
					Translate_Coord(&x, &y);

					if (Culling_Check(x, y, rect, background_coord_layer->data->speed))
					{
						App->render->Blit(coord_tileset->data->texture, x, y, &rect, false, { background_coord_layer->data->speed,  1 });
					}


				}
			}
		}
		background_coord_layer = background_coord_layer->next;
	}
	if (App->player->player.player_tang_mode)
		App->render->DrawQuad({0, 0, data.width * data.tile_width,  data.width * data.tile_width }, 0, 0, 0, 100);


	if (!App->player->player.player_tang_mode) 
	{
		for (int layer_counter = 0; layer_counter < number_of_layers; layer_counter++) {

			//for of every x in one layer
			for (int i = 0; i < coord_layer->data->height; i++) {

				//for of every y in one layer

				for (int j = 0; j < coord_layer->data->width; j++) {
					int n = coord_layer->data->Get(j, i);
					int gid = coord_layer->data->gid[n];
					if (gid != 0) {
						while (ret == false) {
							if (coord_tileset->next != NULL && coord_tileset->next->data->firstgid <= gid) coord_tileset = coord_tileset->next;
							else if (coord_tileset->prev != NULL && coord_tileset->data->firstgid > gid)coord_tileset = coord_tileset->prev;
							else ret = true;
						}
						ret = false;
						SDL_Rect rect = coord_tileset->data->GetRect(coord_layer->data->gid[n]);
						int x = j;
						int y = i;
						Translate_Coord(&x, &y);

						if (Culling_Check(x, y, rect, coord_layer->data->speed)) 
						{
							App->render->Blit(coord_tileset->data->texture, x, y, &rect, false, { coord_layer->data->speed,  1 });
						}


					}
				}
			}
			coord_layer = coord_layer->next;
		}
	}
	else
	{
		for (int layer_counter = 0; layer_counter < data.tang_layers.count(); layer_counter++) {

			//for of every x in one layer
			if (tang_coord_layer != nullptr)
			{
				for (int i = 0; i < tang_coord_layer->data->height; i++) {

					//for of every y in one layer

					for (int j = 0; j < tang_coord_layer->data->width; j++) {
						int n = tang_coord_layer->data->Get(j, i);
						int gid = tang_coord_layer->data->gid[n];
						if (gid != 0) {
							while (ret == false) {
								if (coord_tileset->next != NULL && coord_tileset->next->data->firstgid <= gid) coord_tileset = coord_tileset->next;
								else if (coord_tileset->prev != NULL && coord_tileset->data->firstgid > gid)coord_tileset = coord_tileset->prev;
								else ret = true;
							}
							ret = false;
							SDL_Rect rect = coord_tileset->data->GetRect(tang_coord_layer->data->gid[n]);
							int x = j;
							int y = i;
							Translate_Coord(&x, &y);

							if (Culling_Check(x, y, rect, 1))
							{
								App->render->Blit(coord_tileset->data->texture, x, y, &rect);
							}
						}
					}
				}
				tang_coord_layer = tang_coord_layer->next;
			}
		}
	}



	if (App->input->is_Debug_Mode)
	{
		for (int i = 0; i < App->colliders.collider_list.count(); i++)
		{
			Collider *col = &App->colliders.collider_list[i];
			SDL_Rect *rect = &col->collider_rect;
			if (Culling_Check(rect->x, rect->y, *rect, 1))
			{
				SDL_Color color;
				switch (col->collider_type)
				{

				case WALKEABLE:
					color = {0, 255, 0, 100};
					break;
				case KILL:
					color = { 255, 0, 0, 100};
					break;
				case CLIMB:
					color = { 0, 0, 255, 100};
					break;

				case TANG:
					color = { 0, 100, 200, 100 };
					break;

				}
				App->render->DrawQuad(*rect, color.r, color.g, color.b, color.a);
			}
		}
	}
	int i;
}


// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		item->data->name.Clear();
		App->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* iteml;
	iteml = data.layers.start;
	while (iteml != NULL)
	{
		iteml->data->name.Clear();
		RELEASE(iteml->data->gid);
		RELEASE(iteml->data);
		iteml = iteml->next;
	}
	data.layers.clear();

	p2List_item<MapLayer*>* itemT;
	itemT = data.tang_layers.start;
	while (itemT != NULL)
	{
		itemT->data->name.Clear();
		RELEASE(itemT->data->gid);
		RELEASE(itemT->data);
		itemT = itemT->next;
	}
	data.tang_layers.clear();

	p2List_item<MapLayer*>* itemBG;
	itemBG = data.background_layers.start;
	while (itemBG != NULL)
	{
		itemBG->data->name.Clear();
		RELEASE(itemBG->data->gid);
		RELEASE(itemBG->data);
		itemBG = itemBG->next;
	}
	data.background_layers.clear();


	p2List_item<MapObjectGroup*>* itemO;
	itemO = data.object_layers.start;
	while (itemO != NULL)
	{
		itemO->data->name.Clear();
		itemO->data->properties.clear();
		for (int i = 0; i < itemO->data->objects.count(); i++)
		{
			itemO->data->objects[i]->properties.clear();
			RELEASE(itemO->data->objects[i]);
		}
		itemO->data->objects.clear();
		RELEASE(itemO->data);
		itemO = itemO->next;
	}
	data.object_layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	map_name = file_name;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, set);
		}
		if (set->speed == 1.0f) 
		{
			if (!set->isTang)
			{
				data.layers.add(set);
			}
			else
			{
				data.tang_layers.add(set);
			}
		}
		else
		{
			data.background_layers.add(set);
		}
	}
	for (layer = map_file.child("map").child("objectgroup"); layer && ret; layer = layer.next_sibling("objectgroup"))
	{
		MapObjectGroup* set = new MapObjectGroup();

		if (ret == true)
		{
			ret = LoadObjectGroup(layer, set);
		}
		data.object_layers.add(set);
	}

	if(ret == true)
	{
		
		p2List_item<MapObjectGroup*>* objects_map;
		objects_map = data.object_layers.start;
		while (objects_map != NULL)
		{
			if (objects_map->data->name == "Music && Sound")PrepareMusicSource(objects_map);
			else if (objects_map->data->name == "EndPoint") {
				if (objects_map->data->properties.start->data->name == "isEndPoint"&&objects_map->data->properties.start->data->prop_value.value_bool == true) {
					end_point = objects_map->data->objects.start->data->rect;
				}
			}
			objects_map = objects_map->next;
		}

		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		
		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();


		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();

		App->render->limitNegX = 0; App->render->limitPosX = ((data.width * data.tile_width) * (int)App->win->GetScale()) - App->render->camera.w;
		App->render->limitPosY = 0;  App->render->limitNegY = ((data.height * data.tile_height) * (int)App->win->GetScale()) - App->render->camera.h;

		App->player->killLimit = data.height * data.tile_height;


		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	layer->name = node.attribute("name").as_string();

	if (strcmp(layer->name.GetString(), "Name_Null") == 0) {
		LOG("Error geting layer name");
		return false;
	}
	layer->width = node.attribute("width").as_uint();
	if (layer->width == 0) {
		LOG("Error geting layer width");
		return false;
	}
	layer->height = node.attribute("height").as_uint();
	if (layer->height == 0) {
		LOG("Error geting layer height");
		return false;
	}
	uint total_gid = layer->width * layer->height;
	layer->gid = new uint[total_gid];
	memset(layer->gid, 0, total_gid*sizeof(uint));

	pugi::xml_node tile = node.child("data").child("tile");
	int i = 0;
	while (tile && strcmp(tile.name(),"tile")==0) 
	{


		layer->gid[i] = tile.attribute("gid").as_uint();
		i++;
		tile = tile.next_sibling("tile");
	}

	if ((p2SString)node.child("properties").child("property").attribute("name").as_string() == (p2SString)"speed")
	{
		layer->speed = node.child("properties").child("property").attribute("value").as_float();
	}

	if ((p2SString)node.child("properties").child("property").attribute("name").as_string() == (p2SString)"isTangLayer" && node.child("properties").child("property").attribute("value").as_bool() == true)
		layer->isTang = true;

	return ret;
}

bool j1Map::LoadObjectGroup(pugi::xml_node& node, MapObjectGroup* object)
{
	bool ret = true;
	object->name = node.attribute("name").as_string();

	if (strcmp(object->name.GetString(), "Name_Null") == 0) {
		LOG("Error geting layer name");
		return false;
	}

	pugi::xml_node props1;
	for (props1 = node.child("properties").child("property"); props1 && ret; props1 = props1.next_sibling("property"))
	{
		object_property* set = new object_property();

		set->name = props1.attribute("name").value();
		set->prop_value.value_bool = props1.attribute("value").as_bool();

		object->properties.add(set);
	}

	pugi::xml_node objs;
	for (objs = node.child("object"); objs && ret; objs = objs.next_sibling("object"))
	{
		object_struct* set = new object_struct();

		set->id = objs.attribute("id").as_int();
		set->rect = { objs.attribute("x").as_int(), objs.attribute("y").as_int() , objs.attribute("width").as_int() , objs.attribute("height").as_int() };
		for (pugi::xml_node subNode = objs.child("properties").child("property"); subNode && ret; subNode = subNode.next_sibling("property"))
		{
			object_property *set2 = new object_property();

			set2->name = subNode.attribute("name").value();
			set2->prop_value.value_string = subNode.attribute("value").as_string();


			set->properties.add(set2);
		}


		object->objects.add(set);
	}

	return ret;
}

bool j1Map::Save(pugi::xml_node& data)const {
	pugi::xml_node map_node = data.append_child("map_info");
	map_node.append_attribute("name") = map_name.GetString();
	return true;
}

bool j1Map::Load(pugi::xml_node& data) {
	App->colliders.collider_list.clear();
	App->map->CleanUp();
	App->map->Load(data.child("map_info").attribute("name").as_string());
	App->colliders.LoadColliders();
	return true;
}


void j1Map::PrepareMusicSource(p2List_item<MapObjectGroup*>* objects_map, bool dead) {
	p2List_item<object_property*>* isMusic;
	isMusic = objects_map->data->properties.start;
	while (isMusic != NULL)
	{
		if (isMusic->data->name == "isMusicObjectGroup"&&isMusic->data->prop_value.value_bool == true)
		{
			if (dead == false)App->audio->PlayMusic(objects_map->data->objects.start->data->properties.start->data->prop_value.value_string);
			else App->audio->PlayMusic(objects_map->data->objects.start->data->properties.start->data->prop_value.value_string, 0);
		}
		isMusic = isMusic->next;
	}
}

p2SString j1Map::GetSourceFromID(int id) {
	pugi::xml_node map_node = App->config_file.child("config").child("map").child("maps").child("map");
	do{
		if (map_node.attribute("id").as_int() == id)break;
		else map_node = map_node.next_sibling("map");
	} while (map_node != NULL);
	if (map_node != NULL)return map_node.attribute("source").as_string();
	else return "";
}
