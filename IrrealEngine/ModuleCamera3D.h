#ifndef __MODULE_CAMERA_3D_H__
#define __MODULE_CAMERA_3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"

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

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	void Camera_Rot();
	void FocusBox(AABB & box);
	float* GetViewMatrix();



	bool first_time = false;

private:

	void CalculateViewMatrix();

public:
	
	vec3 X = { 0, 0, 0 }, Y = { 0, 0, 0 }, Z = { 0, 0, 0 }, Position = { 0, 0, 0 }, Reference = { 0, 0, 0 };
	bool focus = false;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};

#endif