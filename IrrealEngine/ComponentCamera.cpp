#include "ComponentCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleImGui.h"
#include "ModuleScene.h"
#include "ModuleMeshLoader.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ImGui/ImGuizmo.h"

#include <math.h>
#include "mmgr/mmgr.h"
//#include "VectorialFunctions.h"
//#include "JsonDoc.h"


//namespace VectF = VectorialFunctions;

ComponentCamera::ComponentCamera(GameObject* parent, float _near, float _far, float _fov)
{
	if (parent != nullptr)
		my_go = parent;
	else
		my_go = App->scene->root;
	UUID = pcg32_random_r(&App->rng);
	type = CAMERA;
	nearDistance = _near;
	farDistance = _far;
	fovy = _fov;

	Reference = float3(0.0f, 0.0f, 0.0f);

	frustum.nearPlaneDistance = nearDistance;
	frustum.farPlaneDistance = farDistance;

	RecalculateFrustrum(App->window->width, App->window->height);
	CalculateViewMatrix();

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.up = float3(0, 1, 0);
	
	frustum.SetWorldMatrix(float3x4::identity);

	ViewMatrix = frustum.ViewMatrix();

	frustum.pos = float3(0.0f, 1.0f, 10.0f);
}

ComponentCamera::~ComponentCamera()
{

}

// -----------------------------------------------------------------
//bool ComponentCamera::Start()
//{
//	LOG("Setting up the camera");
//	bool ret = true;
//
//	frustum.pos = float3(0.0f, 1.0f, 10.0f);
//
//	return ret;
//}

// -----------------------------------------------------------------
bool ComponentCamera::CleanUp()
{
	LOG("Cleaning camera");
	my_go = nullptr;
	
	return true;
}

// -----------------------------------------------------------------
bool ComponentCamera::Update()
{
	UpdateTransform();

	// Recalculate matrix -------------
	if (drawFrustum)
		App->renderer3D->DrawFrustum(frustum);

	
	CalculateViewMatrix();
	return UPDATE_CONTINUE;
}

void ComponentCamera::UpdateUI()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		bool cur = isCurCam;
		ImGui::Checkbox("Is current camera", &cur);

		if (cur != isCurCam)
			App->scene->SetCurCam(this);

		ImGui::Checkbox("Draw Fustum", &drawFrustum);

		ImGui::SliderFloat("FarPlane", &farDistance, nearDistance + 10, 600);
		ImGui::SliderFloat("NearPlane", &nearDistance, 0, farDistance - 10);
		ImGui::SliderFloat("FOV", &fovy, 0, 100);

		frustum.nearPlaneDistance = nearDistance;
		frustum.farPlaneDistance = farDistance;
		RecalculateFrustrum();


		/*ImGui::Separator();
		if (ImGui::Button("Delete"))
			Delete();*/
	}
}

void ComponentCamera::SetInspectorInfo()
{
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		/*ImGui::PushID("CameraActive");
		ImGui::Checkbox("Active", &active);
		ImGui::Spacing();
		ImGui::PopID();*/

		bool cur = isCurCam;
		ImGui::Checkbox("Is current camera", &cur);

		if (cur != isCurCam)
			App->scene->SetCurCam(this);

		ImGui::Checkbox("Draw Fustum", &drawFrustum);

		ImGui::SliderFloat("FarPlane", &farDistance, nearDistance + 10, 600);
		ImGui::SliderFloat("NearPlane", &nearDistance, 0, farDistance - 10);
		ImGui::SliderFloat("FOV", &fovy, 0, 100);

		frustum.nearPlaneDistance = nearDistance;
		frustum.farPlaneDistance = farDistance;
		RecalculateFrustrum();


		/*ImGui::Separator();
		if (ImGui::Button("Delete"))
			Delete();*/
	}
}

void ComponentCamera::CheckInput(float dt)
{

	if (ImGui::IsMouseHoveringAnyWindow()) return;
	

	float3 newPos(0, 0, 0);

	float speed = 8.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 50.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos.z -= speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos.z += speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos.x -= speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos.x += speed;


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) LookAt({ 0,0,0 });

	if (App->input->GetMouseZ() != 0 && !ImGui::IsMouseHoveringAnyWindow())
	{
		float wheelspd = frustum.pos.Length() / 2;
		if (App->input->GetMouseZ() > 0)
			newPos.z -= speed * wheelspd;

		else
			newPos.z += speed * wheelspd;
	}
	frustum.pos -= frustum.front * newPos.z;
	frustum.pos -= frustum.up.Cross(frustum.front) * newPos.x;
 

	Reference -= newPos;

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGuizmo::IsOver())
		Pick((0, 0, 0));

	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;


		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{


			if (App->imgui->focused_go != nullptr)
				Reference = App->imgui->focused_go->global_AABB.CenterPoint();
			else
				Reference = { 0,0,0 };

			if (!lookingAt)
			{
				LookAt(Reference);
				lookingAt = true;
			}


			float3 focus = frustum.pos - Reference;
			transform.rotation = transform.rotation.RotateAxisAngle({ 0,1,0 }, math::DegToRad(dx)) * transform.rotation.RotateAxisAngle(frustum.front.Cross(frustum.up), math::DegToRad(dy));

			focus = transform.rotation.Transform(focus);
			frustum.pos = focus + Reference;


		}
		else
			lookingAt = false;


		float3 left = frustum.front.Cross(frustum.up);
		transform.rotation = transform.rotation.RotateAxisAngle({ 0,1,0 }, math::DegToRad(dx));
		transform.rotation = transform.rotation * transform.rotation.RotateAxisAngle(left, math::DegToRad(dy));
	
		frustum.front = transform.rotation.Mul(frustum.front);
		frustum.up = transform.rotation.Mul(frustum.up);

		transform.position = frustum.pos;

	}
	if (my_go != nullptr)
	{
		ComponentTransform* comp_trans = (ComponentTransform*)(my_go->GetComponent(TRANSFORMATION));
		comp_trans->position = transform.position;
		comp_trans->scale = transform.scale;
		comp_trans->rotation = transform.rotation;

		comp_trans->CalculateMatrix();
	}
	
}


// -----------------------------------------------------------------
void ComponentCamera::Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference)
{
	//this->Position = Position;
	//this->Reference = Reference;


	//Z = float3(Position - Reference).Normalized();
	//X = float3(float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	//Y = Z.Cross(X);

	//if (!RotateAroundReference)
	//{
	//	this->Reference = this->Position;
	//	this->Position += Z * 0.05f;
	//}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ComponentCamera::LookAt(const float3 &Spot)
{
	Reference = Spot;

	float3x3 rotmat = float3x3::LookAt(frustum.front, (Spot - frustum.pos).Normalized(), frustum.up, { 0,1,0 });


	frustum.front = rotmat.MulDir(frustum.front).Normalized();
	frustum.up = rotmat.MulDir(frustum.up).Normalized();

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ComponentCamera::GetViewMatrix() const
{

	return (float*)ViewMatrix.v;
}

float * ComponentCamera::GetPerspMatrix() const
{
	return (float*)frustum.ProjectionMatrix().v;
}

float4x4 ComponentCamera::GetViewMatrixF() const
{
	return ViewMatrix;
}

float4x4 ComponentCamera::GetPerspMatrixF() const
{
	return frustum.ProjectionMatrix();
}

math::Frustum ComponentCamera::GetFrustum() const
{
	return frustum;
}

// -----------------------------------------------------------------
void ComponentCamera::CalculateViewMatrix()
{
	ViewMatrix = frustum.ViewMatrix();
	ViewMatrix.Transpose();
}

float4x4 ComponentCamera::ResizePerspMatrix(int width, int height)
{
	RecalculateFrustrum(width, height);
	return	frustum.ProjectionMatrix();
}

void ComponentCamera::RecalculateFrustrum(int width, int height)
{
	if (width != 0 && height != 0)
	aspectRatio = (float)width / (float)height;

	frustum.verticalFov = math::DegToRad(fovy);

	float ratio = tanf(frustum.verticalFov / 2) * aspectRatio;
	frustum.horizontalFov = 2*atanf(ratio);
	

}

bool ComponentCamera::CheckInside(const AABB m)
{
	math::Sphere sphere;
	sphere.pos = frustum.CenterPoint();
	float3 point[8];
	frustum.GetCornerPoints(point);
	float3 radius = point[0] - sphere.pos;

	for (int i = 0; i < 8; i++)
	{
		if ((point[i] - sphere.pos).Length() > radius.Length())
			radius = point[i] - sphere.pos;
	}
	sphere.r = radius.Length();

	if (sphere.Contains(m))
	{
		float3 corners[8];
		m.GetCornerPoints(corners);
		math::Plane planes[6];
		frustum.GetPlanes(planes);
		uint in = 8;

		for (int p = 0; p < 6; ++p) {

			for (int i = 0; i < 8; ++i) {

				if (planes[p].IsOnPositiveSide(corners[i]))
				{
					in--;
				}
			}

			if (in == 0)
				return false;
			else
				return true;
		}

	}
	return false;
}

void ComponentCamera::UpdateTransform()
{
	if (my_go == nullptr) return;
	transform.position = ((ComponentTransform*)(my_go->GetComponent(TRANSFORMATION)))->position;
	transform.rotation = ((ComponentTransform*)(my_go->GetComponent(TRANSFORMATION)))->rotation;
	
	frustum.pos = transform.position;

	frustum.front = transform.rotation.Mul({ 1,0,0 });
	frustum.up = (transform.rotation.Mul({ 0,1,0 }));

}

Value ComponentCamera::Save(Document::AllocatorType& allocator) const
{
	Value CompArray(kObjectType);
	CompArray.AddMember("Type", type, allocator);
	CompArray.AddMember("Active", active, allocator);
	CompArray.AddMember("UUID", UUID, allocator);
	CompArray.AddMember("NearPlane", frustum.nearPlaneDistance, allocator);
	CompArray.AddMember("FarPlane", frustum.farPlaneDistance, allocator);
	CompArray.AddMember("FOV", fovy, allocator);

	return CompArray;
}

bool ComponentCamera::Load(Document& document)
{
	return true;
}

GameObject* ComponentCamera::Pick(float3* hit_point)
{
	float norm_x = -(1.0f - (float(App->input->Mx) * 2.0f) / (float)App->window->width);
	float norm_y = 1.0f - (float(App->input->My) * 2.0f) / (float)App->window->height;

	LineSegment picking = frustum.UnProjectLineSegment(norm_x, norm_y);

	std::vector<GameObject*> ABBpicked;

	for (std::vector<GameObject*>::iterator it = App->scene->root->go_children.begin(); it != App->scene->root->go_children.end(); ++it)
		(*it)->IsPickedABB(picking, ABBpicked);

	GameObject* selection = nullptr;
	float min_distance = -1;

	for (std::vector<GameObject*>::iterator it = ABBpicked.begin(); it != ABBpicked.end(); ++it)
	{
		for (std::vector<Component*>::iterator cit = (*it)->go_components.begin(); cit != (*it)->go_components.end(); ++cit)
		{
			if ((*cit)->type == MESH)
			{
				LineSegment localSeg(picking);
				localSeg.Transform(((ComponentTransform*)((*it)->GetComponent(TRANSFORMATION)))->matrix_global.Inverted());
				FBXMesh* mesh = ((ComponentMesh*)*cit)->go_mesh;
				Triangle tri;

				for (uint i = 0; i < mesh->num_indices;)
				{
					if (mesh->num_indices - i > 9)
					{
						tri.a.x = mesh->vertices[mesh->indices[i++]*3];
						tri.a.y = mesh->vertices[mesh->indices[i]*3+1];
						tri.a.z = mesh->vertices[mesh->indices[i]*3+2];

						tri.b.x = mesh->vertices[mesh->indices[i++]*3];
						tri.b.y = mesh->vertices[mesh->indices[i]*3+1];
						tri.b.z = mesh->vertices[mesh->indices[i]*3+2];

						tri.c.x = mesh->vertices[mesh->indices[i++]*3];
						tri.c.y = mesh->vertices[mesh->indices[i]*3+1];
						tri.c.z = mesh->vertices[mesh->indices[i]*3+2];

						float distance;
						float3 hit_point;
						if (localSeg.Intersects(tri, &distance, &hit_point))
						{
							if (min_distance == -1 || distance < min_distance)
							{
								min_distance = distance;
								selection = (*it);
							}
						}
					}
					else
						break;
				}
			}
		}
	}

	if (selection != nullptr)
		App->imgui->focused_go = selection;

	return nullptr;
}