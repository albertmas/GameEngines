#ifndef _COMPONENT_TRANSFORM_H_
#define _COMPONENNT_TRANSFORM_H_

#include "Component.h"

class Component;

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	 void DrawInfo() override;
	
	 bool Update() override;


};

#endif