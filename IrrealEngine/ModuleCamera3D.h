#ifndef __MODULE_CAMERA_3D_H__
#define __MODULE_CAMERA_3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Camera.h"

class Camera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool Save(Document& document, FileWriteStream& fws);
	bool Load(Document& document);

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	void Camera_Rot();
	void FocusBox(AABB & box);
	float* GetViewMatrix();

	Camera* GetCurrentCam()const;


	bool first_time = false;



public:
	
	vec3 X = { 0, 0, 0 }, Y = { 0, 0, 0 }, Z = { 0, 0, 0 }, Position = { 0, 0, 0 }, Reference = { 0, 0, 0 };
	bool focus = false;

private:

	Camera * game_camera = nullptr;
	Camera * editor_camera = nullptr;

	std::vector<Camera*> cams_list;

	mat4x4 ViewMatrix, ViewMatrixInverse;
};

#endif