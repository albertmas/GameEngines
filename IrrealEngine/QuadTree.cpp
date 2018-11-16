#include "QuadTree.h"
#include "Application.h"
#include "ModuleSceneLoader.h"
#include "ModuleScene.h"
Quadtree::Quadtree()
{

}

Quadtree::Quadtree(AABB _qtbb, int _currentsubdivisions)
{
	quadtreeboundingbox = new AABB(_qtbb);
	my_subdivision = _currentsubdivisions;

	for (int i = 0; i < 4; i++) {
		childs.push_back(nullptr);
	}
}


Quadtree::~Quadtree()
{
	Clear();
}

void Quadtree::Clear()
{
	RELEASE(quadtreeboundingbox);

	if (childs[0] != nullptr) {
		for (int i = 0; i < 4; i++) {
			childs[i]->Clear();
			childs.clear();
		}
	}

	my_objects.clear();

}

void Quadtree::Insert(GameObject * _go)
{
	if (_go != nullptr) {
		if (quadtreeboundingbox->Intersects(_go->local_AABB) && _go->IsStatic()) {
			if (childs[0] != nullptr) {
				for (int i = 0; i < 4; i++) {

					childs[i]->Insert(_go);

				}

			}
			else {
				my_objects.push_back(_go);
				if (my_objects.size() > maxcapacity) {
					if (childs[0] == nullptr) {
						SubDivide();
					}

					for (int i = 0; i < my_objects.size(); i++) {
						for (int j = 0; j < 4; j++) {
							childs[j]->Insert(my_objects[i]);
						}
					}
					my_objects.clear();
				}

			}

		}
	}

}

void Quadtree::Remove(GameObject * _go)
{
	if (childs[0] != nullptr) {
		for (int i = 0; i < 4; i++) {
			childs[0]->Remove(_go);
		}
	}
	else {
		for (std::vector<GameObject*>::iterator item = my_objects.begin(); item != my_objects.end(); item++) {
			if ((*item) == _go) {
				my_objects.erase(item);
			}
		}
	}
}

void Quadtree::SubDivide()
{
	float lenght_x, lenght_y, lenght_z;
	int nextsubdivision = my_subdivision + 1;
	lenght_x = quadtreeboundingbox->MaxX() - quadtreeboundingbox->MinX();
	lenght_y = quadtreeboundingbox->MaxY() - quadtreeboundingbox->MinY();
	lenght_z = quadtreeboundingbox->MaxZ() - quadtreeboundingbox->MinZ();


	float3 topcenter = { lenght_x / 2,lenght_y,lenght_z / 2 };


	math::AABB tmp({ quadtreeboundingbox->minPoint }, { quadtreeboundingbox->minPoint + topcenter });
	childs[0] = new Quadtree(tmp, nextsubdivision);


	float3 minchild1, maxchild1;
	minchild1 = quadtreeboundingbox->minPoint + float3(lenght_x*0.5f, 0.0f, 0.0f);
	maxchild1 = quadtreeboundingbox->minPoint + float3({ lenght_x,0.0f,lenght_z*0.5f });
	math::AABB tmp1(minchild1, maxchild1);
	childs[1] = new Quadtree(tmp1, nextsubdivision);

	float3 minchild2, maxchild2;
	minchild2 = quadtreeboundingbox->minPoint + float3(0.0f, 0.0f, lenght_z*0.5f);
	maxchild2 = quadtreeboundingbox->minPoint + float3(lenght_x*0.5f, 0.0f, lenght_z);
	math::AABB tmp2(minchild2, maxchild2);
	childs[2] = new Quadtree(tmp2, nextsubdivision);

	float3 minchild3, maxchild3;
	minchild3 = quadtreeboundingbox->minPoint + topcenter;
	maxchild3 = quadtreeboundingbox->minPoint + float3(lenght_x, 0.0f, lenght_z);
	math::AABB tmp3(minchild3, maxchild3);
	childs[3] = new Quadtree(tmp3, nextsubdivision);

}

void Quadtree::CollectIntersections(std::vector<GameObject*>& _goicollidewith, AABB & _goboundingbox)
{
	if (my_objects.empty() == false) {
		if (quadtreeboundingbox->Intersects(_goboundingbox)) {
			if (childs[0] == nullptr) {
				for (int i = 0; i < my_objects.size(); i++) {
					if (_goboundingbox.Intersects(my_objects[i]->local_AABB)) {
						_goicollidewith.push_back(my_objects[i]);
					}
				}
			}
			else {
				for (int i = 0; i < 4; i++) {
					childs[i]->CollectIntersections(_goicollidewith, _goboundingbox);
				}
			}
		}
	}
}

void Quadtree::RenderQuadTree()
{
	if (quadtreeboundingbox != nullptr) {

		if (childs[0] != nullptr) {
			for (int i = 0; i < 4; i++) {
				childs[i]->RenderQuadTree();
			}
		}

		glLineWidth(2.5f);
		glColor3f(1.0f, 1.0f, 0.0f);

		glBegin(GL_LINES);

		glVertex3f(quadtreeboundingbox->CornerPoint(0).x, quadtreeboundingbox->CornerPoint(0).y, quadtreeboundingbox->CornerPoint(0).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(1).x, quadtreeboundingbox->CornerPoint(1).y, quadtreeboundingbox->CornerPoint(1).z);

		glVertex3f(quadtreeboundingbox->CornerPoint(0).x, quadtreeboundingbox->CornerPoint(0).y, quadtreeboundingbox->CornerPoint(0).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(2).x, quadtreeboundingbox->CornerPoint(2).y, quadtreeboundingbox->CornerPoint(2).z);

		glVertex3f(quadtreeboundingbox->CornerPoint(0).x, quadtreeboundingbox->CornerPoint(0).y, quadtreeboundingbox->CornerPoint(0).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(4).x, quadtreeboundingbox->CornerPoint(4).y, quadtreeboundingbox->CornerPoint(4).z);

		glVertex3f(quadtreeboundingbox->CornerPoint(3).x, quadtreeboundingbox->CornerPoint(3).y, quadtreeboundingbox->CornerPoint(3).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(1).x, quadtreeboundingbox->CornerPoint(1).y, quadtreeboundingbox->CornerPoint(1).z);

		glVertex3f(quadtreeboundingbox->CornerPoint(3).x, quadtreeboundingbox->CornerPoint(3).y, quadtreeboundingbox->CornerPoint(3).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(2).x, quadtreeboundingbox->CornerPoint(2).y, quadtreeboundingbox->CornerPoint(2).z);

		glVertex3f(quadtreeboundingbox->CornerPoint(3).x, quadtreeboundingbox->CornerPoint(3).y, quadtreeboundingbox->CornerPoint(3).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(7).x, quadtreeboundingbox->CornerPoint(7).y, quadtreeboundingbox->CornerPoint(7).z);

		glVertex3f(quadtreeboundingbox->CornerPoint(6).x, quadtreeboundingbox->CornerPoint(6).y, quadtreeboundingbox->CornerPoint(6).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(2).x, quadtreeboundingbox->CornerPoint(2).y, quadtreeboundingbox->CornerPoint(2).z);

		glVertex3f(quadtreeboundingbox->CornerPoint(6).x, quadtreeboundingbox->CornerPoint(6).y, quadtreeboundingbox->CornerPoint(6).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(4).x, quadtreeboundingbox->CornerPoint(4).y, quadtreeboundingbox->CornerPoint(4).z);

		glVertex3f(quadtreeboundingbox->CornerPoint(6).x, quadtreeboundingbox->CornerPoint(6).y, quadtreeboundingbox->CornerPoint(6).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(7).x, quadtreeboundingbox->CornerPoint(7).y, quadtreeboundingbox->CornerPoint(7).z);

		glVertex3f(quadtreeboundingbox->CornerPoint(5).x, quadtreeboundingbox->CornerPoint(5).y, quadtreeboundingbox->CornerPoint(5).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(1).x, quadtreeboundingbox->CornerPoint(1).y, quadtreeboundingbox->CornerPoint(1).z);

		glVertex3f(quadtreeboundingbox->CornerPoint(5).x, quadtreeboundingbox->CornerPoint(5).y, quadtreeboundingbox->CornerPoint(5).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(4).x, quadtreeboundingbox->CornerPoint(4).y, quadtreeboundingbox->CornerPoint(4).z);

		glVertex3f(quadtreeboundingbox->CornerPoint(5).x, quadtreeboundingbox->CornerPoint(5).y, quadtreeboundingbox->CornerPoint(5).z);
		glVertex3f(quadtreeboundingbox->CornerPoint(7).x, quadtreeboundingbox->CornerPoint(7).y, quadtreeboundingbox->CornerPoint(7).z);

		glEnd();

		glColor3f(1, 1, 1);
		glLineWidth(1.0f);
	}
}

void Quadtree::RecalculateQuadTree()
{
	if (childs[0] != nullptr) {
		for (int i = 0; i < 4; i++) {
			childs[i]->RemoveMyObjects();

		}
	}
	RemoveMyObjects();
	if (!App->scene->game_objects.empty()) {
		for (int i = 0; i < App->scene->game_objects.size(); i++) {
			Insert(App->scene->game_objects[i]);
		}

	}

}

void Quadtree::RemoveMyObjects()
{
	my_objects.clear();
}


