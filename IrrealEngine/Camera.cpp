#include "Camera.h"
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "ModuleCamera3D.h"
#include "Application.h"
#include "ModuleInput.h"


Camera::Camera()
{

	frustum.pos = (float3::zero);
	frustum.front = (float3::unitZ);
	frustum.up = (float3::unitY);
	SetFOV(80);
	frustum.nearPlaneDistance = 0.5;
	frustum.farPlaneDistance = 100;

	frustum.type = FrustumType::PerspectiveFrustum;

	CreateNewFrustum();
}

Camera::~Camera()
{
}


void Camera::SetFront(const float3 & front)
{
	frustum.pos = front;
}

void Camera::SetUp(const float3 & up)
{
	frustum.up = up;
}

void Camera::SetFOV(const float & new_fov)
{
	if (new_fov <= 0)
	{
		LOG("Can't set a fov of negative value : %d", new_fov);
	}
	else
	{
		frustum.verticalFov = new_fov * DEGTORAD;
		frustum.horizontalFov = math::Atan(aspect_ratio * math::Tan(frustum.verticalFov / 2)) * 2;
	}
}

void Camera::SetFarPlane(const float & new_fp)
{
	frustum.farPlaneDistance = new_fp;
}

void Camera::SetNearPlane(const float & new_np)
{
	frustum.nearPlaneDistance = new_np;
}

void Camera::SetAspectRatio(const float & new_ar)
{
	if (new_ar <= 0)
	{
		LOG("Can't set a fov of negative value : %d", new_ar);
	}
	else
	{
		aspect_ratio = new_ar;
		frustum.horizontalFov = math::Atan(aspect_ratio * math::Tan(frustum.verticalFov / 2)) * 2;
	}
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
	static float4x4 matrix;
	matrix = frustum.ViewMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

float * Camera::GetProjectionMatrix()
{
	static float4x4 matrix;
	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

void Camera::UpdateProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool Camera::IsGameObjectInFrustum(AABB& bb, float3 translation)
{
	bool ret = true;
	int num_vertices = bb.NumVertices();
	for (int i = 0; i < 6; i++)
	{
		int outside_vertices = 0;
		for (int j = 0; j < num_vertices; j++)
		{
			Plane plane = frustum.GetPlane(i);
			if (plane.IsOnPositiveSide(bb.CornerPoint(j) + translation))
				outside_vertices++;
		}
		if (outside_vertices == 8)
			ret = false;

	}
	return ret;
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
	/*this->Position = Position;
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

	CalculateViewMatrix();*/
}

void Camera::LookAt(const float3 &Spot)
{
	Frustum* editor_frustum = &App->camera->editor_camera->frustum;
	float3 direction = Spot - editor_frustum->pos;

	float3x3 matrix = float3x3::LookAt(editor_frustum->front, direction.Normalized(), editor_frustum->up, float3::unitY);

	editor_frustum->front = matrix.MulDir(editor_frustum->front).Normalized();
	editor_frustum->up = matrix.MulDir(editor_frustum->up).Normalized();
}

//void Camera::CalculateViewMatrix()
//{
//	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);
//	ViewMatrixInverse = ViewMatrix.Inverted();
//}

void Camera::UpdatePosition(float3 newpos)
{
	//Position += newpos;
	//Reference += newpos;                     
	frustum.Translate(newpos);
}


void Camera::HandleMouse(const float dt)
{
	

	// Look around
	float dx = -App->input->GetMouseXMotion() * App->camera->GetMouseSensitivity() * dt;
	float dy = -App->input->GetMouseYMotion() * App->camera->GetMouseSensitivity() * dt;



	Frustum* editor_frustum = &App->camera->editor_camera->frustum;

	if (dx != 0)
	{
	
		Quat X_rot = Quat::RotateY(dx);
		editor_frustum->front = X_rot.Mul(editor_frustum->front).Normalized();
		editor_frustum->up = X_rot.Mul(editor_frustum->up).Normalized();
	
	}

	if (dy != 0)
	{
		
		Quat rotation_y = Quat::RotateAxisAngle(editor_frustum->WorldRight(), dy);

		float3 new_up = rotation_y.Mul(editor_frustum->up).Normalized();

		if (new_up.y > 0.0f)
		{
			editor_frustum->up = new_up;
			editor_frustum->front = rotation_y.Mul(editor_frustum->front).Normalized();
		}
	}

}

float3 Camera::Rotate(const float3 & u, float angle, const float3 & v)
{
	return *(float3*)&(float4x4::RotateAxisAngle(v, angle) * float4(u, 1.0f));
}

void Camera::CreateNewFrustum()
{
	frustum.GetCornerPoints(frustum_vertices);
}


void Camera::DrawFrustum()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_LINES);

	glVertex3fv((GLfloat*)&frustum_vertices[1]);
	glVertex3fv((GLfloat*)&frustum_vertices[5]);
	glVertex3fv((GLfloat*)&frustum_vertices[7]);
	glVertex3fv((GLfloat*)&frustum_vertices[3]);

	glVertex3fv((GLfloat*)&frustum_vertices[3]);
	glVertex3fv((GLfloat*)&frustum_vertices[1]);

	glVertex3fv((GLfloat*)&frustum_vertices[4]);
	glVertex3fv((GLfloat*)&frustum_vertices[0]);
	glVertex3fv((GLfloat*)&frustum_vertices[2]);
	glVertex3fv((GLfloat*)&frustum_vertices[6]);

	glVertex3fv((GLfloat*)&frustum_vertices[6]);
	glVertex3fv((GLfloat*)&frustum_vertices[4]);

	glVertex3fv((GLfloat*)&frustum_vertices[5]);
	glVertex3fv((GLfloat*)&frustum_vertices[4]);
	glVertex3fv((GLfloat*)&frustum_vertices[6]);
	glVertex3fv((GLfloat*)&frustum_vertices[7]);

	glVertex3fv((GLfloat*)&frustum_vertices[7]);
	glVertex3fv((GLfloat*)&frustum_vertices[5]);

	glVertex3fv((GLfloat*)&frustum_vertices[0]);
	glVertex3fv((GLfloat*)&frustum_vertices[1]);
	glVertex3fv((GLfloat*)&frustum_vertices[3]);
	glVertex3fv((GLfloat*)&frustum_vertices[2]);

	glVertex3fv((GLfloat*)&frustum_vertices[2]);
	glVertex3fv((GLfloat*)&frustum_vertices[6]);

	glVertex3fv((GLfloat*)&frustum_vertices[3]);
	glVertex3fv((GLfloat*)&frustum_vertices[7]);
	glVertex3fv((GLfloat*)&frustum_vertices[6]);
	glVertex3fv((GLfloat*)&frustum_vertices[2]);

	glVertex3fv((GLfloat*)&frustum_vertices[2]);
	glVertex3fv((GLfloat*)&frustum_vertices[0]);

	glVertex3fv((GLfloat*)&frustum_vertices[0]);
	glVertex3fv((GLfloat*)&frustum_vertices[4]);
	glVertex3fv((GLfloat*)&frustum_vertices[5]);
	glVertex3fv((GLfloat*)&frustum_vertices[1]);

	glEnd();

}


