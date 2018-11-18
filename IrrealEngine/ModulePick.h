#ifndef __ModulePicking_H__
#define __ModulePicking_H__

#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "Primitive.h"

class GameObject;
class Mesh;

template <typename T>
struct Buffer
{
	uint id = 0u;
	uint size = 0u;
	T* buffer = nullptr;
};

class ModulePick : public Module
{
public:
	ModulePick(Application* app, bool start_enabled = true);
	~ModulePick();

	bool Start();
	bool CleanUp();
	update_status Update();

	void DrawRay();
	void CreateRayTest(int x, int y);

	void CheckObjectPicking(GameObject * iterator, const LineSegment & ray, float & smallerDist, GameObject *& nearObject);

	bool CheckMeshTri(Primitive * geometry, LineSegment & rayTransformed, float & smallerDist);

	Triangle SetTrianglePoint(int i);

public:

	Mesh * mesh;
	LineSegment mouse_ray;

};

#endif // !__ModulePicking_H__
