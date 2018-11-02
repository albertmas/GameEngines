#ifndef _COMPONENT_TRANSFORM_H_
#define _COMPONENNT_TRANSFORM_H_

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

class Component;

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	 void DrawInfo() override;
	
	 bool Update() override;


public:

	float3 position = { 0.0f,0.0f,0.0f };

	Quat rotation = { 1.0f,0.0f,0.0f,0.0f };
	float3 rotationVec = { 0.0f,0.0f,0.0f };

	float3 scale = { 1.0f,1.0f,1.0f };
};

#endif