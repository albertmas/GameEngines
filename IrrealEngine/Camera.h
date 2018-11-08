#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "MathGeoLib/MathGeoLib.h"

class Camera {

public:
	Camera();
	~Camera();

	//FRUSTUM 
	bool IsCulling()const;
	void SetCulling(bool culling);
	Frustum GetFrustum()const;

	void SetPosition(const float3& new_pos);
	void SetReference(const float3& new_pos);
	void SetFOV(const float& new_fov);
	void SetFarPlane(const float& new_fp);
	void SetNearPlane(const float& new_np);
	void SetAspectRatio(const float& new_ar);
	float GetFOV()const;
	float GetVerticalFOV()const;
	float GetHorizontalFOV()const;
	float GetFarPlane()const;
	float GetNearPlane()const;
	float GetAspectRatio()const;

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	void CalculateViewMatrix();

	// MOVEMENT

	void UpdatePosition(float3 newpos);
	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3& Spot);
	void HandleMouse();
	float3 X, Y, Z, Position, Reference;

	float3 Rotate(const float3 &u, float angle, const float3 &v);
	
	void CreateNewFrustum();
	

private:
	float4x4 ViewMatrix, ViewMatrixInverse;

private:
	Frustum frustum;
	float	aspect_ratio = 0.0f;
	float   vertical_fov = 0.0f;
	float3 frustum_vertices[8];
	bool	is_culling = true;
};





#endif  __CAMERA_H__