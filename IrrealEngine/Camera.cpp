#include "Camera.h"
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "ModuleCamera3D.h"
#include "Application.h"


Camera::Camera()
{

}

Camera::~Camera()
{
}

void Camera::SetPosition(const float3 & new_pos)
{
	
}
void Camera::SetFOV(const float & new_fov)
{
	
}

void Camera::SetFarPlane(const float & new_fp)
{
	
}

void Camera::SetNearPlane(const float & new_np)
{
	
}

void Camera::SetAspectRatio(const float & new_ar)
{
	
}

float Camera::GetVerticalFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

float Camera::GetHorizontalFOV() const
{
	return frustum.horizontalFov * RADTODEG;
}

float Camera::GetFarPlane() const
{
	return frustum.farPlaneDistance;
}

float Camera::GetNearPlane() const
{
	return frustum.nearPlaneDistance;
}

float Camera::GetAspectRatio() const
{
	return aspect_ratio;
}

bool Camera::IsCulling() const
{
	return is_culling == true;
}

void Camera::SetCulling(bool culling)
{
	is_culling = culling;
}

Frustum Camera::GetFrustum()const
{
	return frustum;
}