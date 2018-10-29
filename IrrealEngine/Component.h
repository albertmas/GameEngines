#ifndef _COMPONENT_H_
#define _COMPONENT_H_


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
	

	bool comp_active = false;
	GameObject* my_go = nullptr;

};

enum COMP_TYPE {
	MESH,
	MATERIAL,
	TRANSFORMATION,
	
};


#endif _COMPONENT_H_
