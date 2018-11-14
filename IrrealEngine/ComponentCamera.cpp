#include "ComponentCamera.h"
#include "ModuleCamera3D.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "MathGeoLib/MathGeoLib.h"
#include "GameObject.h"


ComponentCamera::ComponentCamera()
{
	cam = new Camera();
	type = CAMERA;
	
}

bool ComponentCamera::Start()
{
	ComponentTransform* trans = (ComponentTransform*)my_go->GetComponent(COMP_TYPE::TRANSFORMATION);
	UpdatePos();

	return true;
}

bool ComponentCamera::Update()
{
	if (cam->frustrum_draw)
	{
		cam->DrawFrustum();
	}
	cam->CreateNewFrustum();
	return true;
}


ComponentCamera::~ComponentCamera()
{
}

Camera * ComponentCamera::GetCamera()
{
	return cam;
}

void ComponentCamera::UpdatePos()
{
	/*ComponentTransform* trans = (ComponentTransform*)my_go->GetComponent(COMP_TYPE::TRANSFORMATION);
	Transform my_owner_trans = trans->transform;
	cam->frustum.pos = my_owner_trans.pos;
	float3 eul = my_owner_trans.rot_euler;
	cam->frustum.front = *(float3*)&(float4x4::RotateAxisAngle({ 1,0,0 }, eul.x * DEGTORAD) * float4({ 0,0,-1 }, 1.0f));
	cam->frustum.up = *(float3*)&(float4x4::RotateAxisAngle({ 1,0,0 }, eul.x * DEGTORAD) * float4({ 0,1,0 }, 1.0f));
	cam->frustum.front = *(float3*)&(float4x4::RotateAxisAngle({ 0,1,0 }, eul.y * DEGTORAD) * float4(cam->frustum.front, 1.0f));
	cam->frustum.up = *(float3*)&(float4x4::RotateAxisAngle({ 0,1,0 }, eul.y * DEGTORAD) * float4(cam->frustum.up, 1.0f));
	cam->frustum.front = *(float3*)&(float4x4::RotateAxisAngle({ 0,0,1 }, eul.z * DEGTORAD) * float4(cam->frustum.front, 1.0f));
	cam->frustum.up = *(float3*)&(float4x4::RotateAxisAngle({ 0,0,1 }, eul.z * DEGTORAD) * float4(cam->frustum.up, 1.0f));
*/
}





