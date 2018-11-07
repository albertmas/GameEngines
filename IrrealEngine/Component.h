#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"


class GameObject;
enum COMP_TYPE {
	NONE,
	MESH,
	TRANSFORMATION,
	TEXTURE,
	CAMERA
};
class Component
{
public:

	Component()
	{}
	Component(GameObject* parent)
	{}
	virtual ~Component()
	{}
	
	virtual void Enable()
	{}
	virtual bool Update() { return true; }
	
	virtual void Disable()
	{}

	virtual void SetInspectorInfo()
	{}

	

public:

	bool comp_active = true;
	GameObject* my_go = nullptr;
	COMP_TYPE type = NONE;

};

#endif