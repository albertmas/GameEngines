#include "ModulePick.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleImGui.h"

ModulePicking::ModulePicking(Application* app, bool start_enabled) : Module(start_enabled)
{
}

ModulePicking::~ModulePicking()
{}

bool ModulePicking::Start()
{
	bool ret = true;

	return ret;
}

bool ModulePicking::CleanUp()
{

	return true;
}

update_status ModulePicking::Update()
{

	
	return UPDATE_CONTINUE;
}

void ModulePicking::CheckObjectPicking(GameObject* iterator, const LineSegment &ray, float &smallerDist, GameObject* &nearObject)
{
	
}

bool ModulePicking::CheckMeshTri(Primitive * geometry, LineSegment &ray, float &smallerDist)
{
	bool ret = false;
	
	return ret;
}

float3 ModulePicking::SetTrianglePoint(Buffer<float> vertex, Buffer<uint> index, uint idIndex)
{
	float3 newPoint;
	newPoint.x = vertex.buffer[index.buffer[idIndex] * 3];
	newPoint.y = vertex.buffer[index.buffer[idIndex] * 3 + 1];
	newPoint.z = vertex.buffer[index.buffer[idIndex] * 3 + 2];

	return newPoint;
}