#include "QuadTree.h"

Quadtree::Quadtree()
{
}

Quadtree::Quadtree(AABB limits, int currentsubdivisions)
{
	QT_Box = limits;
	my_subdivision = currentsubdivisions;
}

Quadtree::~Quadtree()
{
}

void Quadtree::Insert(GameObject * go)
{
	if (QT_Box.Intersects(go->global_AABB)) {
		if (!childs.empty()) {
			for (auto qtChildIt : childs) {
				qtChildIt->Insert(go);
			}
		}
		else if (my_subdivision > 2) {
			my_objects.push_back(go);
			return;
		}
		else {

			my_objects.push_back(go);
			if (1 < my_objects.size())
			{
				if (childs.empty())
					SubDivide();

				for (auto gobjsIt : my_objects)
				{
					for (auto childsIt : childs)
						childsIt->Insert(gobjsIt);
				}

				my_objects.clear();
			}

		}
	}
}

void Quadtree::Intersect(std::vector<GameObject*>& go, const AABB& goboundingbox)
{
	if (goboundingbox.Intersects(QT_Box))
	{
		for (std::vector<GameObject*>::const_iterator it = this->my_objects.begin(); it != this->my_objects.end(); ++it)
		{
			if (goboundingbox.Intersects((*it)->global_AABB))
				go.push_back(*it);
		}
		for (int i = 0; i < 4; ++i)
			if (childs[i] != nullptr) childs[i]->Intersect(go, goboundingbox);
	}
}

void Quadtree::Clear()
{
	QT_Box.SetNegativeInfinity();
	my_subdivision = 0;
	if (!childs.empty()) {
		for (auto it : childs)
		{
			it->Clear();
			it = nullptr;
		}
		childs.clear();
	}
	for (auto j : my_objects)
	{
		j = nullptr;
	}
	my_objects.clear();
}



void Quadtree::SubDivide()
{
	float x = QT_Box.Size().x;
	float y = QT_Box.Size().y;
	float z = QT_Box.Size().z;

	float3 centerXZ = { QT_Box.HalfSize().x, y, QT_Box.HalfSize().z };

	//NW
	childs.push_back(new Quadtree(AABB(QT_Box.minPoint, QT_Box.minPoint + centerXZ), my_subdivision + 1));

	//NE
	float3 neMin = QT_Box.minPoint + float3(QT_Box.HalfSize().x, 0.0f, 0.0f);
	childs.push_back(new Quadtree(AABB(neMin, neMin + centerXZ), my_subdivision + 1));

	//SW
	float3 swMin = QT_Box.minPoint + float3(0.0f, 0.0f, QT_Box.HalfSize().z);
	childs.push_back(new Quadtree(AABB(swMin, swMin + centerXZ), my_subdivision + 1));

	//SE
	childs.push_back(new Quadtree(AABB(QT_Box.maxPoint - centerXZ, QT_Box.maxPoint), my_subdivision + 1));
}

void Quadtree::DrawQuadtree() {

	float3 corners[8];
	QT_Box.GetCornerPoints(corners);
	DrawBox(corners, Red, 5.f);

	for (auto it : childs)
	{
		it->DrawQuadtree();
	}
}

void Quadtree::DrawBox(float3 * vertices, Color color, float lineWidth)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(lineWidth);
	glColor3f(color.r, color.g, color.b);

	glBegin(GL_LINES);
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[3]);

	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[1]);

	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[4]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[7]);

	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[5]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[0]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[1]);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
	glEnd();
}
