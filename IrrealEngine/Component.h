#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"


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

	enum COMP_TYPE {
		MESH,
		TRANSFORMATION,
		TEXTURE,
		MATERIAL
	};

public:

	bool comp_active = true;
	GameObject* my_go = nullptr;

};

#endif