#ifndef _COMPONENT_CAMERA_H_
#define _COMPONENT_CAMERA_H_	
#include "Component.h"

class ModuleCamera3D;


class ComponentCamera : public Component
{
public:

	ComponentCamera();
	virtual ~ComponentCamera();
	bool Start();
	bool Update()override;
	ModuleCamera3D* GetCamera();
	void UpdatePos();

public:
	ModuleCamera3D * cam;

};

#endif _COMPONENT_CAMERA_H_