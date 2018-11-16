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
	Quadtree(AABB limits, int currentsubdivisions);
	~Quadtree();

	void Insert(GameObject* gameObject);
	

	void Intersect(std::vector<GameObject*>& go, const AABB& goboundingbox);
	void SubDivide();

	void Clear();


public:

	AABB QT_Box;
	int my_subdivision;
	std::vector<Quadtree*> childs;
	std::vector<GameObject*> my_objects;
	int maxcapacity = 1;

};



#endif //!QUADTREE
