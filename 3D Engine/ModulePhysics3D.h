#pragma once
#include "Module.h"
#include "Globals.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class DebugDrawer;
struct PhysBody3D;
struct PhysVehicle3D;
struct VehicleInfo;
class vec3;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void CreateSphere(const float3 position, int radius);
	void CreateTriangle(const float3 &a_, const float3 &b_, const float3 &c_);
	std::list<float2> GetSphereCollisions();
	

	void AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB);
	void AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisS, const vec3& axisB, bool disable_collision = false);
	//Usefull functions
	
	bool debug;


	std::vector<Sphere> spheres_list;

	
};

