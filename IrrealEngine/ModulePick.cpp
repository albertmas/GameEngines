#include "ModulePick.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleImGui.h"

ModulePick::ModulePick(Application* app, bool start_enabled) : Module(start_enabled)
{
}

ModulePick::~ModulePick()
{}

bool ModulePick::Start()
{
	bool ret = true;

	return ret;
}

bool ModulePick::CleanUp()
{

	return true;
}

void ModulePick::DrawRay()
{
	glBegin(GL_LINES);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(10.0f);
	glDisable(GL_CULL_FACE);
	glColor3f(255, 0, 255);

	glVertex3fv((GLfloat*)&mouse_ray.a);
	glVertex3fv((GLfloat*)&mouse_ray.b);

	glEnd();
	glLineWidth(1.0f);
	glColor3f(255, 255, 255);
	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ModulePick::CreateRayTest(int x, int y)
{

	float n_x = (((x - App->scene->GetPos().x) / App->scene->GetSize().x));
	float n_y = (((y - App->scene->GetPos().y) / App->scene->GetSize().y));

	n_x -= 0.5;
	n_x *= 2;

	n_y -= 0.5;
	n_y *= -2;

	LineSegment picking;
	if (n_x > -1 && n_x < 1 && n_y > -1 && n_y < 1)
	{
		picking =App->camera->GetEditorCam()->frustum.UnProjectLineSegment(n_x, n_y);
		mouse_ray = picking;
	}
	//Check Collisions
	if (picking.Length() != 0)
		App->scene->ClickSelect(picking);
}

update_status ModulePick::Update()
{

	
	return UPDATE_CONTINUE;
}

void ModulePick::CheckObjectPicking(GameObject* iterator, const LineSegment &ray, float &smallerDist, GameObject* &nearObject)
{
	
}

bool ModulePick::CheckMeshTri(Primitive * geometry, LineSegment &ray, float &smallerDist)
{
	bool ret = false;
	
	return ret;
}

Triangle ModulePick::SetTrianglePoint(int i)
{
	Triangle ret;
	float3 a = mesh->vertices[mesh->indices[i]];
	float3 b = mesh->vertices[mesh->indices[i + 1]];
	float3 c = mesh->vertices[mesh->indices[i + 2]];

	ret.a = a;
	ret.b = b;
	ret.c = c;

	return ret;
}