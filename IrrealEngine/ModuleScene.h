#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"
#include "QuadTree.h"
#include <list>

class GameObject;
class Quadtree;

class ModuleScene :	public Module
{
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Init(Document& document);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool Save(Document& document, FileWriteStream& fws);
	bool Load(Document& document);

	void Draw();
	GameObject* CreateGameObject();
	GameObject* CreateCamera();
	void SetGlobalMatrix(GameObject* gameobject);

	void DrawGOBoundingBoxes();

	Quadtree* GlobalQuadTree;

	GameObject* Closest_go(std::vector<GameObject*> gameobjects, LineSegment line);
	void ClickSelect(LineSegment mouse_ray);
	GameObject* GetSelected();
	ImVec2 GetPos()const;
	ImVec2 GetSize()const;


public:
	std::vector<GameObject*> game_objects;
	GameObject* root = nullptr;

private:
	ImVec2 pos;
	ImVec2 size;
};

#endif