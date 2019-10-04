#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------

struct Layer {
	p2SString name = "Name_Null";
	uint width = 0u;
	uint height = 0u;
};

struct Tileset {
	uint firstgid = 0u;
	p2SString name = "Name_Null";
	uint tilewidth = 0u;
	uint tileheight = 0u;
	uint spacing = 0u;
	uint margin = 0u;
	p2SString image_source = "Name_Null";
	uint image_width = 0u;
	uint image_height = 0u;
};

enum orientation {
	OR_NULL,
	OR_ORTHOGONAL,
	OR_ISOMETRIC,
	OR_HEXAGONAL
};
enum renderorder {
	REOR_NULL,
	REOR_RIGHT_DOWN,
	REOR_LEFT_DOWN,
	REOR_RIGHT_UP,
	REOR_LEFT_UP
};

// TODO 1: Create a struct needed to hold the information to Map node
struct Map_Information {
	orientation orient=OR_NULL;
	renderorder renderord=REOR_NULL;
	uint width = 0u;
	uint height = 0u;
	uint tilewidth = 0u;
	uint tileheight = 0u;
	uint nextobjectid = 0u;
};


// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);



private:

	bool load_map_information(pugi::xml_node&);

	bool load_tileset(int, pugi::xml_node&);

	bool load_layer(int, pugi::xml_node&);

public:

	// TODO 1: Add your struct for map info as public for now
	Map_Information map_information;
	Tileset* tileset;
	Layer* layer;
	int	number_of_tilesets;
	int number_of_layers;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__