#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
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

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)
	for (int i = 0; i < number_of_tilesets; i++) {
		SDL_Rect rect = { 0, 0, tileset[i].image_width, tileset[i].image_height };
		App->render->Blit(App->tex->Load(tileset[i].image_source.GetString()), 0, 0, &rect);
	}

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map
	delete[] tileset;
	delete[] layer;

	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data
		ret=load_map_information(map_file.child("map"));
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	number_of_tilesets = 0;
	pugi::xml_node tileset_node = map_file.child("map").child("tileset");
	for (pugi::xml_node tileset_node_counter = tileset_node; tileset_node_counter; tileset_node_counter = tileset_node_counter.next_sibling("tileset")) {
		number_of_tilesets++;
	}
	tileset = new Tileset[number_of_tilesets];
	for (int i = 0; i < number_of_tilesets; i++) {
		if (i != 0)tileset_node = tileset_node.next_sibling("tileset");
		ret = load_tileset(i, tileset_node);
		if (ret == false)return ret;
	}

	number_of_layers = 0;
	pugi::xml_node layer_node = map_file.child("map").child("layer");
	for (pugi::xml_node layer_node_counter = layer_node; layer_node_counter; layer_node_counter = layer_node_counter.next_sibling("layer")) {
		number_of_layers++;
	}
	layer = new Layer[number_of_layers];
	for (int i = 0; i < number_of_layers; i++) {
		if (i != 0)layer_node = layer_node.next_sibling("layer");
		ret = load_layer(i, layer_node);
		if (ret == false)return ret;
	}

	if(ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %u  height: %u", map_information.width, map_information.height);
		LOG("tile_width: %u  tile_height: %u", map_information.tilewidth, map_information.tileheight);
		for (int i = 0; i < number_of_tilesets;i++) {
			LOG("Tileset ----");
			LOG("name: %s  firstgid: %u", tileset[i].name.GetString(), tileset[i].firstgid);
			LOG("tile_width: %u  tile_height: %u", tileset[i].tilewidth, tileset[i].tileheight);
			LOG("spacing: %u  margin: %u", tileset[i].spacing, tileset[i].margin);
		}
		for (int i = 0; i < number_of_layers; i++) {
			LOG("Layer ----");
			LOG("name: %s", layer[i].name.GetString());
			LOG("width: %u  height: %u", layer[i].width, layer[i].height);
		}
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::load_map_information(pugi::xml_node& node_sent) {
	if (strcmp(node_sent.attribute("orientation").as_string(), "orthogonal")==0) map_information.orient = OR_ORTHOGONAL;
	else if (strcmp(node_sent.attribute("orientation").as_string(), "isometric") == 0) map_information.orient = OR_ORTHOGONAL;
	else if (strcmp(node_sent.attribute("orientation").as_string(), "exagonal") == 0) map_information.orient = OR_ORTHOGONAL;
	else {
		LOG("COULDN'T READ ORIENTATION: %s", node_sent.attribute("orientation").as_string());
		return false;
	}
	if (strcmp(node_sent.attribute("renderorder").as_string(), "right-down") == 0) map_information.renderord = REOR_RIGHT_DOWN;
	else if (strcmp(node_sent.attribute("renderorder").as_string(), "right-up") == 0) map_information.renderord = REOR_RIGHT_UP;
	else if (strcmp(node_sent.attribute("renderorder").as_string(), "left-down") == 0) map_information.renderord = REOR_LEFT_DOWN;
	else if (strcmp(node_sent.attribute("renderorder").as_string(), "left-up") == 0) map_information.renderord = REOR_LEFT_UP;
	else {
		LOG("COULDN'T READ RENDER ORDER: %s", node_sent.attribute("renderorder").as_string());
		return false;
	}
	map_information.width = node_sent.attribute("width").as_uint();
	if (map_information.width == 0u) {
		LOG("COULDN'T READ RENDER WIDTH: %u", node_sent.attribute("width").as_uint());
		return false;
	}
	map_information.height = node_sent.attribute("height").as_uint();
	if (map_information.height == 0u) {
		LOG("COULDN'T READ RENDER HEIGHT: %u", node_sent.attribute("height").as_uint());
		return false;
	}
	map_information.tilewidth = node_sent.attribute("tilewidth").as_uint();
	if (map_information.tilewidth == 0u) {
		LOG("COULDN'T READ RENDER TILE WIDTH: %u", node_sent.attribute("tilewidth").as_uint());
		return false;
	}
	map_information.tileheight = node_sent.attribute("tileheight").as_uint();
	if (map_information.tileheight == 0u) {
		LOG("COULDN'T READ RENDER TILE HEIGHT: %u", node_sent.attribute("tileheight").as_uint());
		return false;
	}
	map_information.nextobjectid = node_sent.attribute("nextobjectid").as_uint();
	if (map_information.nextobjectid == 0u) {
		LOG("COULDN'T READ RENDER NEXT OBJECT ID: %u", node_sent.attribute("nextobjectid").as_uint());
		return false;
	}
	return true;
}

bool j1Map::load_tileset(int tileset_id, pugi::xml_node& node_sent) {
	tileset[tileset_id].firstgid = node_sent.attribute("firstgid").as_uint();
	if (tileset[tileset_id].firstgid == 0u) {
		LOG("COULDN'T READ FIRST GID: %u", node_sent.attribute("firstgid").as_uint());
		return false;
	}
	tileset[tileset_id].name = node_sent.attribute("name").as_string();
	if (strcmp(tileset[tileset_id].name.GetString(), "Name_Null")==0) {
		LOG("COULDN'T READ NAME: %s", node_sent.attribute("name").as_string());
		return false;
	}
	tileset[tileset_id].tilewidth = node_sent.attribute("tilewidth").as_uint();
	if (tileset[tileset_id].tilewidth == 0u) {
		LOG("COULDN'T READ TILE WIDTH: %u", node_sent.attribute("tilewidth").as_uint());
		return false;
	}
	tileset[tileset_id].tileheight = node_sent.attribute("tileheight").as_uint();
	if (tileset[tileset_id].tileheight == 0u) {
		LOG("COULDN'T READ TILE HEIGHT: %u", node_sent.attribute("tileheight").as_uint());
		return false;
	}
	tileset[tileset_id].spacing = node_sent.attribute("spacing").as_uint();
	if (tileset[tileset_id].spacing == 0u) {
		LOG("COULDN'T READ SPACING: %u", node_sent.attribute("spacing").as_uint());
		return false;
	}
	tileset[tileset_id].margin = node_sent.attribute("margin").as_uint();
	if (tileset[tileset_id].margin == 0u) {
		LOG("COULDN'T READ MARGIN: %u", node_sent.attribute("margin").as_uint());
		return false;
	}
	node_sent = node_sent.child("image");
	tileset[tileset_id].image_source = node_sent.attribute("source").as_string();
	if (strcmp(tileset[tileset_id].image_source.GetString(), "Name_Null") == 0) {
		LOG("COULDN'T READ IMAGE SOURCE: %s", node_sent.attribute("source").as_string());
		return false;
	}
	tileset[tileset_id].image_width = node_sent.attribute("width").as_uint();
	if (tileset[tileset_id].image_width == 0u) {
		LOG("COULDN'T READ IMAGE WIDTH: %u", node_sent.attribute("width").as_uint());
		return false;
	}
	tileset[tileset_id].image_height = node_sent.attribute("height").as_uint();
	if (tileset[tileset_id].image_height == 0u) {
		LOG("COULDN'T READ IMAGE HEIGHT: %u", node_sent.attribute("height").as_uint());
		return false;
	}
	return true;
}

bool j1Map::load_layer(int layer_id, pugi::xml_node& node_sent) {
	layer[layer_id].name = node_sent.attribute("name").as_string();
	if (strcmp(layer[layer_id].name.GetString(), "Name_Null") == 0) {
		LOG("COULDN'T READ NAME: %s", node_sent.attribute("name").as_string());
		return false;
	}
	layer[layer_id].width = node_sent.attribute("width").as_uint();
	if (layer[layer_id].width == 0u) {
		LOG("COULDN'T READ WIDTH: %u", node_sent.attribute("width").as_uint());
		return false;
	}
	layer[layer_id].height = node_sent.attribute("height").as_uint();
	if (layer[layer_id].height == 0u) {
		LOG("COULDN'T READ HEIGHT: %u", node_sent.attribute("height").as_uint());
		return false;
	}
	return true;
}