#ifndef _GameObject_
#define _GameObject_

#include "Globals.h"
#include "Component.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool Init();
	void Update();
	bool CleanUp();



public:
	bool active = false;
	std::string name = "";
	//std::vector<Component*> components;
};

#endif