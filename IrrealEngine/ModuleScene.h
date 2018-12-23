#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"
#include <list>

class GameObject;
class ComponentCamera;

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
	void SetGlobalMatrix(GameObject* gameobject);

	void SetCurCam(ComponentCamera* cam);
	ComponentCamera* GetCurCam() const;
	ComponentCamera* GetGhostCam() const;

public:
	std::vector<GameObject*> game_objects;
	GameObject* root = nullptr;
	GameObject* audiolistenerdefault = nullptr;
	GameObject* centaur = nullptr;
	GameObject* train = nullptr;

private:
	ComponentCamera* ghostcam = nullptr;
	ComponentCamera* currentCam = nullptr;

	bool train_forward = true;
};

#endif