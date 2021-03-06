#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModulePhysics3D.h"
#include "ModuleImGui.h"
#include "Globals.h"

#include "mmgr/mmgr.h"


//ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
//{
//	CalculateViewMatrix();
//
//	X = vec3(1.0f, 0.0f, 0.0f);
//	Y = vec3(0.0f, 1.0f, 0.0f);
//	Z = vec3(0.0f, 0.0f, 1.0f);
//
//	first_time = true;
//
//	Position = vec3(0.0f, -5.0f, 10.0f);
//	Reference = vec3(0.0f, 0.0f, 0.0f);
//}
//
//ModuleCamera3D::~ModuleCamera3D()
//{}
//
//// -----------------------------------------------------------------
//bool ModuleCamera3D::Start()
//{
//	LOG("Setting up the camera");
//	bool ret = true;
//
//	return ret;
//}
//
//// -----------------------------------------------------------------
//bool ModuleCamera3D::CleanUp()
//{
//	LOG("Cleaning camera");
//
//	return true;
//}
//
//// -----------------------------------------------------------------
//update_status ModuleCamera3D::Update(float dt)
//{
//	if (ImGui::IsMouseHoveringAnyWindow())
//		return UPDATE_CONTINUE;
//
//	////Debug camera
//	vec3 newPos(0, 0, 0);
//	float speed = 10.0f * dt;
//	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
//		speed *= 2;
//
//
//	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
//	{
//		
//
//		if(App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos -= Y * speed;
//		if(App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += Y * speed;
//
//		if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
//		if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;
//		
//		if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
//		if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;
//
//		Position += newPos;
//		Reference += newPos;
//		Camera_Rot();
//
//		focus = true;
//	}
//
//	if (App->input->GetMouseZ() > 0)
//	{
//		newPos -= Z * speed * 3;
//
//		focus = true;
//		Position += newPos;
//		Reference += newPos;
//	}
//	if (App->input->GetMouseZ() < 0) {
//		newPos += Z * speed * 3;
//
//		focus = true;
//		Position += newPos;
//		Reference += newPos;
//	}
//
//	// Look at mesh (currently centered)
//	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
//		LookAt({ 0,0,0 });
//		// Adapt position depending on size of the mesh
//	}
//
//	// Mouse motion ----------------
//
//	if ((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT)))
//	{
//		Camera_Rot();
//
//		Position = Reference + Z * length(Position);
//		LookAt({ 0,0,0 });
//	}
//
//	
//
//	// Recalculate matrix -------------
//	CalculateViewMatrix();
//	
//	return UPDATE_CONTINUE;
//}
//
//bool ModuleCamera3D::Save(Document& document, FileWriteStream& fws)const
//{
//	bool ret = true;
//
//	return ret;
//}
//
//bool ModuleCamera3D::Load(Document& document)
//{
//	bool ret = true;
//
//	return ret;
//}
//// -----------------------------------------------------------------
//void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
//{
//	this->Position = Position;
//	this->Reference = Reference;
//
//	Z = normalize(Position - Reference);
//	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
//	Y = cross(Z, X);
//
//	if(!RotateAroundReference)
//	{
//		this->Reference = this->Position;
//		this->Position += Z * 0.05f;
//	}
//
//	CalculateViewMatrix();
//}
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::LookAt( const vec3 &Spot)
//{
//	Reference = Spot;
//
//	Z = normalize(Position - Reference);
//	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
//	Y = cross(Z, X);;
//
//	CalculateViewMatrix();
//}
//
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::Move(const vec3 &Movement)
//{
//	Position += Movement;
//	Reference += Movement;
//
//	CalculateViewMatrix();
//}
//
//// -----------------------------------------------------------------
//float* ModuleCamera3D::GetViewMatrix()
//{
//	return &ViewMatrix;
//}
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::CalculateViewMatrix()
//{
//	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
//	ViewMatrixInverse = inverse(ViewMatrix);
//}
//
//void ModuleCamera3D::Camera_Rot()
//{
//	// Look around
//	int dx = -App->input->GetMouseXMotion();
//	int dy = -App->input->GetMouseYMotion();
//
//	float Sensitivity = 0.25f;
//	
//	if (dx != 0)
//	{
//		float DeltaX = (float)dx * Sensitivity;
//
//		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
//		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
//		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
//	}
//
//	if (dy != 0)
//	{
//		float DeltaY = (float)dy * Sensitivity;
//
//		Y = rotate(Y, DeltaY, X);
//		Z = rotate(Z, DeltaY, X);
//
//		if (Y.y < 0.0f)
//		{
//			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
//			Y = cross(Z, X);
//		}
//	}
//	
//}
//void ModuleCamera3D::FocusBox(AABB & box)
//{
//	Position.x = box.maxPoint.x ;
//	Position.y = box.maxPoint.y  ;
//	Position.z = box.maxPoint.z +8 ;
//	vec3 focus_position;
//	focus_position.x = box.CenterPoint().x;
//	focus_position.y = box.CenterPoint().y;
//	focus_position.z = box.CenterPoint().z;
//	Z = normalize(Position - focus_position);
//	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
//	Y = cross(Z, X);
//	CalculateViewMatrix();
//
//}