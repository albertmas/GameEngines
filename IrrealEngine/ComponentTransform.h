#ifndef _COMPONENT_TRANSFORM_H_
#define _COMPONENT_TRANSFORM_H_

#include "Component.h"


class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* gameobject);
	~ComponentTransform() override;

	bool Update() override;
	void SetInspectorInfo() override;

	Value Save(Document::AllocatorType& allocator) const override;
	bool Load(Document& document) override;

	void CalculateMatrix();

public:
	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::one;

	float4x4 matrix_local = float4x4::identity;
	float4x4 matrix_global = float4x4::identity;
};

#endif