#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModulePhysics3D.h"
#include "ModuleImGui.h"


ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	CalculateViewMatrix();

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 0.0f, 30.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
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
	////Debug camera

	
		float3 newPos(0,0,0);
		float speed = 50.0f * dt;
		if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 80.0f * dt;

		if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
		if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

		if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos.z -= speed;
		if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos.z += speed;
		
		if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos.x -= speed;
		if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos.x += speed;

		Position += newPos;
		Reference += newPos;

	//	// Mouse motion ----------------

	//	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	//	{
	//		int dx = -App->input->GetMouseXMotion();
	//		int dy = -App->input->GetMouseYMotion();

	//		float Sensitivity = 0.25f;

	//		Position -= Reference;

	//		if(dx != 0)
	//		{
	//			float DeltaX = (float)dx * Sensitivity;

	//			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		}

	//		if(dy != 0)
	//		{
	//			float DeltaY = (float)dy * Sensitivity;

	//			Y = rotate(Y, DeltaY, X);
	//			Z = rotate(Z, DeltaY, X);

	//			if(Y.y < 0.0f)
	//			{
	//				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//				Y = cross(Z, X);
	//			}
	//		}

	//		Position = Reference + Z * length(Position);
	//	}

	//	// Recalculate matrix -------------
		
	//}
		CalculateViewMatrix();
	
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
	this->Position = Position;
	this->Reference = Reference;

	float3 aux_z = Position - Reference;
	Z = aux_z.Normalized();
	X = ((float3(0.0f, 1.0f, 0.0f).Cross(Z).Normalized()));
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3 &Spot)
{
	Reference = Spot;
	float3 aux_z = Position - Reference;
	Z = aux_z.Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return (float*)ViewMatrix.v;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}
