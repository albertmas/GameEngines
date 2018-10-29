#pragma once

#include "Globals.h"
#include "GameObject.h"

class GameObject;

class Component
{
public:

	Component()
	{}
	Component(GameObject* parent)
	{}
	virtual ~Component()
	{}
	virtual void DrawInfo() 
	{}
	virtual void Enable()
	{}
	virtual bool Update() { return true; }
	
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

