#pragma once

#include "Globals.h"
#include "GameObject.h"

class GameObject;

class Component
{
public:

	Component()
	{}
	virtual ~Component()
	{}
	
	virtual void Enable()
	{}
	virtual void Update()
	{}
	virtual void Disable()
	{}


public:

	//Game Object
	enum COMP_TYPE {
		MESH,
		MATERIAL,
		TRANSFORMATION,
		TEXTURE
	};

	bool comp_active = false;
	GameObject* my_go = nullptr;

};

