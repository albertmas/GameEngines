#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModulePhysics3D.h"
#include "ModuleImGui.h"
#include "Globals.h"
#include "ModuleCamera3D.h"
#include "Camera.h"
#include "ComponentCamera.h"
#include "GameObject.h"


ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	editor_camera = new Camera();
	cams_list.push_back(editor_camera);

	GetCurrentCam()->CalculateViewMatrix();

	GetCurrentCam()->X = float3(1.0f, 0.0f, 0.0f);
	GetCurrentCam()->Y = float3(0.0f, 1.0f, 0.0f);
	GetCurrentCam()->Z = float3(0.0f, 0.0f, 1.0f);

	first_time = true;

	GetCurrentCam()->Position = float3(0.0f, 5.0f, 10.0f);
	GetCurrentCam()->Reference = float3(0.0f, 0.0f, 0.0f);


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
	
	
	return UPDATE_CONTINUE;
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
void ModuleCamera3D::LookAt( const float3 &Spot)
{
	editor_camera->LookAt(Spot);
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	float3 newPos(0, 0, 0);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= editor_camera->Z.Mul(Movement);
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += editor_camera->Z.Mul(Movement);

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= editor_camera->X.Mul(Movement);
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += editor_camera->X.Mul(Movement);

	editor_camera->UpdatePosition(newPos);
}

void ModuleCamera3D::MoveCam(const float3 &speed)
{
	
	float3 newPos(speed.x, speed.y, speed.z);


	editor_camera->SetPosition(newPos);
	editor_camera->SetReference(newPos);
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return GetCurrentCam()->GetViewMatrix();
}

Camera * ModuleCamera3D::GetCurrentCam() const
{
	return editor_camera;
}


// -----------------------------------------------------------------

void ModuleCamera3D::HandleMouse()
{

}

void ModuleCamera3D::Camera_Rot()
{
	LookAt({ 0,0,0 });	
}



void ModuleCamera3D::CameraMovement(float dt)
{

	float3 newPos(0, 0, 0);
	float speed = 10.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 2;


	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		Move({ speed,speed,speed });

	}

	if (App->input->GetMouseZ() > 0)
	{
		newPos -= Z * speed * 3;

		focus = true;
		GetCurrentCam()->Position += newPos;
		GetCurrentCam()->Reference += newPos;
	}
	if (App->input->GetMouseZ() < 0) {
		newPos += Z * speed * 3;

		focus = true;
		GetCurrentCam()->Position += newPos;
		GetCurrentCam()->Reference += newPos;
	}

	// Look at mesh (currently centered)
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		LookAt({ 0,0,0 });
		// Adapt position depending on size of the mesh
	}

	// Mouse motion ----------------

	if ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT)))
	{
		Camera_Rot();

		
		LookAt({ 0,0,0 });
	}



	// Recalculate matrix -------------
	GetCurrentCam()->CalculateViewMatrix();



}

void ModuleCamera3D::FocusBox(AABB & box)
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
	GetCurrentCam()->CalculateViewMatrix();

}