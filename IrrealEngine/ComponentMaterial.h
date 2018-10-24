#ifndef COMPONENT_MATERIAL_H_
#define COMPONENT_MATERIAL_H_

#include "Component.h"
#include "Globals.h"


class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* gameobject);
	~ComponentMaterial() override;
};

#endif