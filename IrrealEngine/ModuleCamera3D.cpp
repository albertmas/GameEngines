#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModulePhysics3D.h"
#include "ModuleImGui.h"
#include "Globals.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModulePick.h"
#include "Camera.h"
#include "ComponentCamera.h"
#include "GameObject.h"


ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{

}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (ImGui::IsMouseHoveringAnyWindow())
		return UPDATE_CONTINUE;


	CameraMovement(dt);

	bool mouse_picking_working = true;
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && CheckMouseInWindow(App->input->GetMouseX(), App->input->GetMouseY()) && mouse_picking_working)
	{
		App->ray->CreateRayTest(App->input->GetMouseX(), App->input->GetMouseY());
	}


	return UPDATE_CONTINUE;
}

void ModuleCamera3D::StartEditorCam()
{
	editor_camera = new Camera();

}



bool ModuleCamera3D::Save(Document& document, FileWriteStream& fws)
{
	bool ret = true;

	return ret;
}

bool ModuleCamera3D::Load(Document& document)
{
	bool ret = true;

	return ret;
}
// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	editor_camera->Look(Position, Reference, RotateAroundReference);
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3 &Spot)
{
	editor_camera->LookAt(Spot);
}


// -----------------------------------------------------------------


void ModuleCamera3D::WheelMove(const float & mouse_speed, int direction)
{
	float3 newPos(0, 0, 0);

	if (direction == 1)
		newPos += editor_camera->frustum.front * mouse_speed;
	else
		newPos -= editor_camera->frustum.front * mouse_speed;

	if (!newPos.IsZero())
		editor_camera->frustum.Translate(newPos);

}

void ModuleCamera3D::MoveCam(const float3 &speed)
{

	editor_camera->frustum.Translate(speed);

	/*float3 newPos(speed.x, speed.y, speed.z);


	editor_camera->SetPosition(newPos);
	editor_camera->SetReference(newPos);*/
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return GetCurrentCam()->GetViewMatrix();
}

Camera * ModuleCamera3D::GetCurrentCam() const
{
	Camera* ret = nullptr;
	if (current_cam == nullptr)
	{
		LOG("No current Camera for Game");
	}
	else
	{
		if (current_cam->HasCam())
			ret = current_cam->GetCamera();
	}
	return ret;
}

void ModuleCamera3D::NewCamera()
{
	Camera* aux = current_cam->GetCamera();
	aux = new Camera();

}

void ModuleCamera3D::SetCurrentCam(GameObject * cam)
{
	if (cam != nullptr)
		current_cam = cam;
}

Camera * ModuleCamera3D::GetEditorCam() const
{
	if (editor_camera != nullptr)
		return editor_camera;
	else
		return nullptr;
}

// -----------------------------------------------------------------

void ModuleCamera3D::HandleMouse(const float dt)
{
	editor_camera->HandleMouse(dt);

}

float ModuleCamera3D::GetMouseSensitivity() const
{
	return mouse_sensitivity;
}
void ModuleCamera3D::Camera_Rot(const float dt)
{

	HandleMouse(dt);

	//GetCurrentCam()->Position = GetCurrentCam()->Reference + GetCurrentCam()->Z * mult(GetCurrentCam()->Position);// need to adapt this line so it orbits

	LookAt({ 0,0,0 });

}

void ModuleCamera3D::Move(const float &Movement)
{
	float3 newPos(0, 0, 0);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += editor_camera->frustum.front * Movement;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= editor_camera->frustum.front * Movement;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= editor_camera->frustum.WorldRight() * Movement;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += editor_camera->frustum.WorldRight() * Movement;

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y -= Movement;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y += Movement;
	if (!newPos.IsZero())
		editor_camera->frustum.Translate(newPos);
}

void ModuleCamera3D::CameraMovement(float dt)
{

	float3 newPos(0, 0, 0);
	float speed = 10.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 2;


	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		HandleMouse(dt);
		Move(speed);

	}

	int wheel = App->input->GetMouseZ();
	float wheel_speed = wheel_speed_base * dt * 100;
	if (App->input->GetMouseZ() != 0)
		WheelMove(wheel_speed, wheel);

	// Look at mesh (currently centered)
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		LookAt({ 0,0,0 });
		// Adapt position depending on size of the mesh
	}

	// Mouse motion ----------------

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT))
	{
		Camera_Rot(dt);
	

	}




}

void ModuleCamera3D::FocusBox(AABB & box, float3 transform)
{
	Position.x = box.maxPoint.x;
	Position.y = box.maxPoint.y;
	Position.z = box.maxPoint.z + 8;
	float3 focus_position;
	focus_position.x = box.CenterPoint().x;
	focus_position.y = box.CenterPoint().y;
	focus_position.z = box.CenterPoint().z;
	//Z = normalize(Position - focus_position);
	//	X = Normalize(Cross(float3(0.0f, 1.0f, 0.0f), Z));
	//	Y = Cross(Z, X);
	//GetCurrentCam()->CalculateViewMatrix();
	editor_camera->frustum.pos.Set(Position.x, Position.y, Position.z);

	float3 look_at_pos{ 0,0,0 };

	look_at_pos.x = box.CenterPoint().x + transform.x;
	look_at_pos.y = box.CenterPoint().y + transform.y;
	look_at_pos.z = box.CenterPoint().z + transform.z;

	LookAt(look_at_pos);

}

float ModuleCamera3D::mult(const float3 &u)
{
	return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}

bool ModuleCamera3D::CheckMouseInWindow(int x, int y)
{
	ImVec2 pos_w = App->scene->GetPos();
	ImVec2 size_w = App->scene->GetSize();
	return (x > pos_w.x && y > pos_w.y && x < pos_w.x + size_w.x && y < pos_w.y + size_w.y);

}