#ifndef _COMPONENT_CAMERA_H_
#define _COMPONENT_CAMERA_H_

#include "Component.h"
#include "ComponentTransform.h"


class ComponentTransform;

class ComponentCamera :	public Component
{
public:
	ComponentCamera(GameObject* parent = nullptr, float _near = 0.5f, float _far = FAR_PLANE_DISTANCE, float fov = 60.0f);
	~ComponentCamera();

	//bool Start();
	bool Update();
	bool CleanUp();
	void SetInspectorInfo() override;
	Value Save(Document::AllocatorType& allocator) const override;
	bool Load(Document& document) override;


	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	float* GetViewMatrix() const;
	float* GetPerspMatrix() const;
	math::Frustum GetFrustum() const;
	float4x4 GetViewMatrixF() const;
	float4x4 GetPerspMatrixF() const;
	float4x4 ResizePerspMatrix(int width, int heigth);
	void RecalculateFrustrum(int width = 0, int height = 0);
	void CheckInput(float dt);
	bool CheckInside(const AABB bb);
	void UpdateTransform();
	void CalculateViewMatrix();
	GameObject* Pick(float3* hit_point);

public:
	float3 Reference = float3::zero;
	ComponentTransform transform = nullptr;
	bool locked = false;
	bool drawFrustum = true;
	bool isCurCam = false;
	float nearDistance = 0.5f;
	float farDistance = FAR_PLANE_DISTANCE;
	Frustum frustum;
	float fovy = 60.0f;

private:

	float4x4 ViewMatrix, ViewMatrixInverse;
	float aspectRatio = 0;
	bool lookingAt = false;
};

#endif //_COMPONENT_CAMERA_H_