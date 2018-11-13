#ifndef __MODULE_CAMERA_3D_H__
#define __MODULE_CAMERA_3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Camera.h"

class Camera;
class GameObject;

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
	void Move(const float &Movement);
	void Camera_Rot(const float dt);
	void MoveCam(const float3 &speed);
	void CameraMovement(float dt);
	void WheelMove(const float &mouse_speed, int direction);
	void HandleMouse(const float dt);
	float GetMouseSensitivity() const;
	void FocusBox(AABB & box, float3 transform);
	float* GetViewMatrix();
	float mult(const float3 &u);

	Camera* GetCurrentCam()const;
	void StartEditorCam();
	void StartGameCam();


	bool first_time = false;



public:

	Camera * game_camera = nullptr;
	Camera * editor_camera = nullptr;

	std::vector<Camera*> cams_list;

	float3 X = { 0, 0, 0 }, Y = { 0, 0, 0 }, Z = { 0, 0, 0 }, Position = { 0, 0, 0 }, Reference = { 0, 0, 0 };
	bool focus = false;


private:
	float mouse_sensitivity = 0.2f;
	float wheel_speed_base = 4.0f;

private:

	float4x4 ViewMatrix, ViewMatrixInverse;
};

#endif