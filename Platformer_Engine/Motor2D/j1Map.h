//Code by Miquel Suau && Arnau Falagueras

#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include"p2DynArray.h"


struct object_property
{
	p2SString name = "Name_Null";
	union value
	{
		bool value_bool = false;
		int value_int;
		float value_float;
		const char* value_string;

	}prop_value;

};
struct object_struct {

	int id = 0;
	SDL_Rect rect = {0, 0, 0, 0};
	p2List<object_property*> properties;
};

struct MapLayer {

	p2SString name = "Name_Null";
	uint width = 0u;
	float speed = 1.0f;
	uint height = 0u;
	uint* gid = nullptr;
	bool isTang = false;
	inline uint Get(int x, int y) const 
	{
		return (y * width) + x;
	}
};
struct MapObjectGroup 
{
	p2SString name = "Name_Null";

	p2List<object_property*> properties;
	p2List<object_struct*> objects;
	
};


// ----------------------------------------------------
struct TileSet
{

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
	inline SDL_Rect GetRect(int id) {
		int n = id - firstgid;
		int x = n % num_tiles_width;
		x = ((x * tile_width) + (x * spacing)) + margin;
		int y = n / num_tiles_width;
		y = ((y * tile_height) + (y * spacing)) + margin;
		return { x,y,tile_width,tile_height };
	}
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	p2List<MapLayer*>	tang_layers;
	p2List<MapObjectGroup*> object_layers;
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

	//Save
	bool Save(pugi::xml_node&) const;

	//Load
	bool Load(pugi::xml_node&);

	//Get source of music
	void PrepareMusicSource(p2List_item<MapObjectGroup*>*, bool dead = false);

	void Translate_Coord(int* x, int* y) {
		*x *= data.tile_width;
		*y *= data.tile_height;
	}

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadObjectGroup(pugi::xml_node& node, MapObjectGroup* object);

public:

	MapData data;
	int culling_offset = 0;
	SDL_Rect end_point;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	p2SString			map_name;
	bool				map_loaded;
};

#endif // __j1MAP_H__