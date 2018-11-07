#ifndef _GameObject_H_
#define _GameObject_H_

#include "Globals.h"
#include "Component.h"

class Component;
enum COMP_TYPE;
class Camera;
class GameObject
{
public:
	GameObject(GameObject* parent, const char* name);
	~GameObject();

	bool Init();
	void Update();
	bool CleanUp();

	void Draw();
	//void DrawBB(const AABB& BB, vec3 color) const;

	Component* CreateComponent(COMP_TYPE type);
	Component* GetComponent(COMP_TYPE type);

public:
	bool go_active = true;
	bool go_static = false;
	std::string go_name = "";
	std::vector<Component*> go_components;
	GameObject* go_parent = nullptr;
	std::vector<GameObject*> go_children;
};

#endif