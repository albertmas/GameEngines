#ifndef _GameObject_H_
#define _GameObject_H_



class Component;
class ComponentTransform;
enum COMP_TYPE;

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool Init();
	void Update();
	bool CleanUp();

	void RemoveComponent(Component component);
	Component* CreateComponent(COMP_TYPE type);

	ComponentTransform* CreateComponentTransform();


public:

	bool go_active = false;

	std::string go_name = "";
	std::vector<Component*> go_components;
	std::vector<GameObject*> go_children;

	GameObject* go_parent = nullptr;
	
};

#endif