#ifndef _COMPONENT_CAMERA_H_
#define _COMPONENT_CAMERA_H_	
#include "Component.h"

class Camera;


class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* gameobject);
	ComponentCamera();
	virtual ~ComponentCamera();
	bool Start();
	bool Update()override;
	Camera* GetCamera();
	void UpdatePos();

public:
	Camera * cam;

};

#endif _COMPONENT_CAMERA_H_