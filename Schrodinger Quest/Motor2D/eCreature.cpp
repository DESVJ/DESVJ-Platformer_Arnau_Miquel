#include"eCreature.h"
#include"EntityManager.h"

pugi::xml_node eCreature::LoadAnimationFromTMX(pugi::xml_node* animation_node, Animation* anim, const char* name)
{
	//Load objectgroups from tmx
	pugi::xml_node objectgroup;
	pugi::xml_node* correctNodeGroup = nullptr;
	for (objectgroup = animation_node->child("objectgroup"); objectgroup; objectgroup = objectgroup.next_sibling("objectgroup"))
	{

		for (pugi::xml_node obj_prop = objectgroup.child("properties").first_child(); obj_prop; obj_prop = obj_prop.next_sibling("property"))
		{
			if ((p2SString)obj_prop.attribute("value").value() == (p2SString)name)
			{
				correctNodeGroup = &objectgroup;
				if ((p2SString)obj_prop.next_sibling().attribute("name").as_string() == (p2SString)"speed")
					anim->speed = obj_prop.next_sibling().attribute("value").as_float();
				break;
			}
		}
		if (correctNodeGroup != nullptr)
			break;

	}


	pugi::xml_node ret;
	//Check for correct node
	if (correctNodeGroup)
	{
		//Load all animation frames
		for (pugi::xml_node obj = correctNodeGroup->child("object"); obj; obj = obj.next_sibling("object"))
		{
			if (obj.child("properties").child("property").attribute("value").as_bool() == true)
			{
				anim->PushBack({ obj.attribute("x").as_int(),
				obj.attribute("y").as_int(),
				obj.attribute("width").as_int(),
				obj.attribute("height").as_int() });

				ret = obj;

			}
		}
	}
	return ret;
}