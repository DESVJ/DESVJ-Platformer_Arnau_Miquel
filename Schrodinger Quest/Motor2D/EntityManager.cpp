#include "EntityManager.h"

EntityManager::EntityManager()
{
	name.create("entity_manager");
}

//Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& a)
{
	for (int i = 0; i < entities->count(); i++)
	{
		entities->At(i)->data->Awake(a);
	}
	return true;
}

// Called before the first frame
bool EntityManager::Start()
{
	for (int i = 0; i < entities->count(); i++)
	{
		entities->At(i)->data->Start();
	}
	return true;
}

// Called each loop iteration
bool EntityManager::PreUpdate()
{
	for (int i = 0; i < entities->count(); i++)
	{
		entities->At(i)->data->PreUpdate();
	}
	return true;
}

// Called each loop iteration
bool EntityManager::Update(float dt)
{
	for (int i = 0; i < entities->count(); i++)
	{
		entities->At(i)->data->Update(dt);
	}
	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	for (int i = entities->count() - 1; i >= 0; i--)
	{
		entities->del(entities->At(i));
	}
	entities->clear();
	return true;
}

//Called when loading the game
bool EntityManager::Load(pugi::xml_node& n)
{
	for (int i = 0; i < entities->count(); i++)
	{
		entities->At(i)->data->Load(n);
	}
	return true;
}

//Called when saving the game
bool EntityManager::Save(pugi::xml_node& s) const
{
	for (int i = 0; i < entities->count(); i++)
	{
		entities->At(i)->data->Save(s);
	}
	return true;
}

//Called when creating a new Entity
Entity* EntityManager::CreateEntity(Types type) 
{
	static_assert(Types::unknown == (Types)4, "Types need update");
	Entity* ret = nullptr;
	switch (type)
	{
	case Types::player:
		break;

	case Types::enemy_ground:
		break;
	case Types::enemy_air:
		break;
	}

	if (ret != nullptr)
	{
		entities->add(ret);
	}
	return ret;
}

//Called when deleting a new Entity
bool EntityManager::DeleteEntity(Entity* e)
{
	int n=entities->find(e);
	if (n == -1)return false;
	else
	{
		entities->del(entities->At(n));
		return true;
	}
}