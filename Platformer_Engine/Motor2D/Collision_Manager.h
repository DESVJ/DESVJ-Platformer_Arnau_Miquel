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

	void MoveObject(SDL_Rect* currentPoint, p2Point<int> increment) 
	{
		//currentPoint->y += increment.y;
		//for itinerate all Colliders of the layer
		//look up collider type
		//currentpos + increment == colision?

		currentPoint->x += increment.x;
		currentPoint->y += increment.y;

		SDL_Rect prediction = *currentPoint;
		prediction.x += increment.x;
		prediction.y += increment.y;
		for (int i = 0; i < collider_list.count(); i++)
		{


			if (prediction.x >= collider_list[i].collider_rect.x && prediction.x <= collider_list[i].collider_rect.x + collider_list[i].collider_rect.w
				&& prediction.y >= collider_list[i].collider_rect.y && prediction.y <= collider_list[i].collider_rect.y + collider_list[i].collider_rect.h)
				LOG("Collision");


			//if (increment.x > 0)
			//{
			//	//Moving right
			//}
			//else
			//{
			//	//Moving left
			//}

			//if (increment.y > 0) 
			//{
			//	//Moving down
			//}
			//else
			//{
			//	//Moving up
			//}

		}
	}

};


#endif // __j1COLLISION_H__