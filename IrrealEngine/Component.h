#ifndef _COMPONENT_H_
#define _COMPONENT_H_


class GameObject;

enum COMP_TYPE {

	EMPTY = 0,
	MESH,
	MATERIAL,
	TRANSFORMATION,

};

class Component
{
public:

	Component()
	{}
	Component(GameObject* gameobject, COMP_TYPE type) :my_go(gameobject), type(type) 
	{}
	virtual ~Component()
	{}
	virtual void DrawInfo() 
	{}
	virtual bool Update() { return true; }

	virtual void Enable()
	{}
	virtual void Disable()
	{}


public:

	//Game Object
	

	bool comp_active = false;
	GameObject* my_go = nullptr;
	COMP_TYPE type = EMPTY;

};




#endif _COMPONENT_H_
