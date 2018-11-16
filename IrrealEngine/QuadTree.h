#ifndef _QUADTREE_H_
#define _QUADTREE_H_
#include "Application.h"
#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"
#include <vector>


class Quadtree
{
public:

	Quadtree();
	Quadtree(AABB limits);
	~Quadtree();

	void Insert(GameObject* gameObject);
	

	void Intersect(std::vector<GameObject*>& go);

	void Clear();


public:

	AABB QT_Box;


};








#endif //!QUADTREE
