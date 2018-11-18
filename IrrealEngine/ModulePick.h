#ifndef __ModulePicking_H__
#define __ModulePicking_H__

#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "Primitive.h"

class GameObject;


template <typename T>
struct Buffer
{
	uint id = 0u;
	uint size = 0u;
	T* buffer = nullptr;
};

class ModulePicking : public Module
{
public:
	ModulePicking(Application* app, bool start_enabled = true);
	~ModulePicking();

	bool Start();
	bool CleanUp();
	update_status Update();

	void CheckObjectPicking(GameObject * iterator, const LineSegment & ray, float & smallerDist, GameObject *& nearObject);

	bool CheckMeshTri(Primitive * geometry, LineSegment & rayTransformed, float & smallerDist);

	float3 SetTrianglePoint(Buffer<float> vertex, Buffer<uint> index, uint idIndex);

public:
	RayLine rayDraw;


};

#endif // !__ModulePicking_H__
