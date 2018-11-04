#ifndef _COMPONENT_TRANSFORM_H_
#define _COMPONENNT_TRANSFORM_H_

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"


class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* gameobject);
	~ComponentTransform() override;

	void SetInspectorInfo() override;

public:
	float3 position = { 0, 0, 0 };
	Quat rotation = { 0, 0, 0, 0 };
	float3 scale = { 0, 0, 0 };
};

#endif