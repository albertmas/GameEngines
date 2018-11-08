#include "Camera.h"
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "ModuleCamera3D.h"
#include "Application.h"
#include "ModuleInput.h"


Camera::Camera()
{
	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	

	Position = float3(0.0f, -5.0f, 10.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
}

Camera::~Camera()
{
}

void Camera::SetPosition(const float3 & new_pos)
{
	
}

void Camera::SetReference(const float3 & new_pos)
{
	Reference = new_pos;
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

float* Camera::GetViewMatrix()
{
	return  &ViewMatrix[0][0];
}

float * Camera::GetProjectionMatrix()
{
	return nullptr;
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

void Camera::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	float3 diff = Position - Reference;
	Z = diff.Normalized();
	float3 YcrossZ = float3(0.0f, 1.0f, 0.0f).Cross(Z);
	X = YcrossZ.Normalized();
	Y = Z.Cross(X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

void Camera::LookAt(const float3 &Spot)
{
	Reference = Spot;

	float3 diff = Position - Reference;
	Z = diff.Normalized();
	float3 YcrossZ = float3(0.0f, 1.0f, 0.0f).Cross(Z);
	X = YcrossZ.Normalized();
	Y = Z.Cross(X);

	CalculateViewMatrix();
}

void Camera::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}

void Camera::UpdatePosition(float3 newpos)
{
	Position += newpos;
	Reference += newpos;
}


void Camera::HandleMouse()
{
	// Look around
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.25f;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;

		X = Rotate(X, DeltaX, float3(0.0f, 1.0f, 0.0f));
		Y = Rotate(Y, DeltaX, float3(0.0f, 1.0f, 0.0f));
		Z = Rotate(Z, DeltaX, float3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Y = Rotate(Y, DeltaY, X);
		Z = Rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = Cross(Z, X);
		}
	}

}

float3 Camera::Rotate(const float3 & u, float angle, const float3 & v)
{
	return *(float3*)&(float4x4::RotateAxisAngle(v, angle) * float4(u, 1.0f));
}