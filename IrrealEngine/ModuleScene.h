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
	~ModuleScene() override;

	bool Init(Document& document) override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	bool Save(Document& document, FileWriteStream& fws)const override;
	bool Load(Document& document) override;

	bool SaveScene(const char* file);
	bool LoadScene(const char* file);
	void CleanScene();
	void CleanChildrenGO(GameObject* child);

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