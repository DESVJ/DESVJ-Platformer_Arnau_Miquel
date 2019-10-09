#ifndef __j1COLLISION_H__
#define __j1COLLISION_H__

#include "j1Module.h"
#include"p2Point.h"
#include"j1Map.h"
#include"p2Log.h"
#include"p2SString.h"
#include"p2List.h"

struct Collider 
{
	SDL_Rect collider_rect = {0, 0, 0, 0};
	p2SString collider_type;
};

struct Collider_Manager
{

	MapLayer* collider_layer = nullptr;
	p2List<Collider> collider_list;

	void MoveObject(p2Point<int>* currentPoint, p2Point<int> increment) 
	{

		currentPoint->x += increment.x;


		//LOG("Moving");
		//for itinerate all Colliders of the layer
		//look up collider type
		//currentpos + increment == colision?

		//for (int i = 0; i < collider_list.count(); i++)
		//{
		//	//if (increment.x != 0)
		//	//{

		//	//}

		//	//if (increment.y != 0)
		//	//{


		//	//}
		//	//Is collider aligned with the object in X?
		//	if(currentPoint->x >= collider_list[i].collider_rect.x && currentPoint->x <= collider_list[i].collider_rect.x + collider_list[i].collider_rect.w
		//		&&
		//		currentPoint->y <= collider_list[i].collider_rect.y)
		//	{
		//		if (increment.y != 0)
		//		{
		//			if (currentPoint->y + increment.y < collider_list[i].collider_rect.y)
		//			{
		//				currentPoint->y += increment.y;
		//			}
		//		}
		//	}


		//}




	}

};


#endif // __j1COLLISION_H__