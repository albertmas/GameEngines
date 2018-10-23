#pragma once

class GameObject;

enum COMP_TYPE {
	MESH,
	TRANSFORMATION,
	TEXTURE,
	MATERIAL
};

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

	bool comp_active = false;
	GameObject* my_go = nullptr;

};

