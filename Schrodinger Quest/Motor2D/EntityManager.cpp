#include "EntityManager.h"
#include "eSnakeEnemy.h"
#include "eBatEnemy.h"
#include"ePotion.h"
#include"eCoin.h"

EntityManager::EntityManager()
{
	name.create("entity_manager");
	Player = (j1Player*)CreateEntity(Types::player);
}

//Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& a)
{
	for (unsigned int i = 0; i < entities.count(); i++)
	{
		entities.At(i)->data->Awake(a.child(entities.At(i)->data->name.GetString()));
	}
	return true;
}

// Called before the first frame
bool EntityManager::Start()
{
	for (unsigned int i = 0; i < entities.count(); i++)
	{
		entities.At(i)->data->Start();
	}
	return true;
}

// Called each loop iteration
bool EntityManager::PreUpdate()
{
	if (Player->respawn == true) {
		for (unsigned int i = 1; i < entities.count(); i++)
		{
			eEnemy* c = (eEnemy*)entities.At(i)->data;
			c->alive = true;
			c->en_state = Enemy_State::idle;
		}
	}
	for (unsigned int i = 0; i < entities.count(); i++)
	{
		entities.At(i)->data->PreUpdate();
	}
	return true;
}

// Called each loop iteration
bool EntityManager::Update(float dt)
{
	for (unsigned int i = 0; i < entities.count(); i++)
	{
		entities.At(i)->data->Update(dt);
	}
	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	for (int i = entities.count() - 1; i >= 0; i--)
	{
		entities.At(i)->data->CleanUp();
		entities.del(entities.At(i));
	}
	entities.clear();
	return true;
}

//Called when loading the game
bool EntityManager::Load(pugi::xml_node& n)
{
	pugi::xml_node n2 = n;
	for (unsigned int i = 0; i < entities.count(); i++)
	{
		n2 = n.child(entities.At(i)->data->name.GetString());
		while (n2.attribute("id").as_int() != i) {
			n2 = n2.next_sibling(entities.At(i)->data->name.GetString());
			assert(n2.attribute("id").as_int() != 0, "Load could not function properly");
		};
		entities.At(i)->data->Load(n2);
	}
	return true;
}

//Called when saving the game
bool EntityManager::Save(pugi::xml_node& s) const
{
	pugi::xml_node s2 = s;
	for (unsigned int i = 0; i < entities.count(); i++)
	{
		s2 = s.append_child(entities.At(i)->data->name.GetString());
		entities.At(i)->data->Save(s2);
		s2.append_attribute("id") = i;
	}
	return true;
}

//Called when creating a new Entity
Entity* EntityManager::CreateEntity(Types type) 
{
	static_assert(Types::unknown == (Types)6, "Types need update");
	Entity* ret = nullptr;
	pugi::xml_document	info_file;
	pugi::xml_document info_file2;

	switch (type)
	{
	case Types::player:
		ret = new j1Player(Types::player);
		break;

	case Types::enemy_ground:
		info_file.load_file("textures/Enemy_Sprites/snake.tmx");
		ret = new eSnakeEnemy(Types::enemy_ground, info_file.child("map"));
		break;

	case Types::enemy_air:
		info_file.load_file("textures/Enemy_Sprites/bat.tmx");
		ret = new eBatEnemy(Types::enemy_air, info_file.child("map"));
		break;

	case Types::healing_potion:
		//load healing potion
		ret = new ePotion(Types::healing_potion, info_file.child("map"));
		break;

	case Types::coins:
		ret = new eCoins(Types::healing_potion, info_file.child("map"));
		break;

	}
	info_file2.load_file("config.xml");
	if (ret != nullptr)
	{
		entities.add(ret);
		if (type == Types::enemy_ground)		
			ret->Awake(info_file2.child("config").child("entity_manager").child("enemy_info").child("ground_enemy"));
		else if(type == Types::enemy_air)
			ret->Awake(info_file2.child("config").child("entity_manager").child("enemy_info").child("fly_enemy"));
		else if (type == Types::healing_potion) 
			ret->Awake(info_file2.child("config").child("entity_manager").child("pickups").child("potion"));
		else if (type == Types::coins)
			ret->Awake(info_file2.child("config").child("entity_manager").child("pickups").child("coins"));
	

	}
	return ret;
}

//Called when deleting a new Entity
bool EntityManager::DeleteEntity(Entity* e)
{
	int n=entities.find(e);
	if (n == -1)return false;
	else
	{
		entities.At(n)->data->CleanUp();
		entities.del(entities.At(n));
		return true;
	}
}

void EntityManager::RespawnCreatures() 
{

	for (unsigned int i = 0; i < entities.count(); i++)
	{
		eCreature* creature = (eCreature*)entities[i];
		creature->collider->collider_rect.x = creature->spawn.x;
		creature->collider->collider_rect.y = creature->spawn.y;
		creature->alive = true;
	}



}

void EntityManager::Deactivate(){
	for (int i = entities.count() - 1; i >= 0; i--)
	{
		if (entities.At(i)->data != Player) {
			entities.At(i)->data->CleanUp();
			entities.del(entities.At(i));
		}
	}
	Player->creating_player = true;
	active = false;
}