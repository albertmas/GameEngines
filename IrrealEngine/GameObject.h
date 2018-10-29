#ifndef _GameObject_H_
#define _GameObject_H_

#include "Globals.h"
#include "Component.h"

class Component;
enum COMP_TYPE;

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool Init();
	void Update();
	bool CleanUp();

	void RemoveComponent(COMP_TYPE type);

	Component* CreateComponent(Component component);


public:

	bool go_active = false;

	std::string go_name = "";
	std::vector<Component*> go_components;
	std::vector<GameObject*> go_children;

	GameObject* go_parent = nullptr;
	
};

#endif