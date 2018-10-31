#ifndef _MODULE_PHYSICS_
#define _MODULE_PHYSICS_
#include "Module.h"



class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(bool start_enabled = true);
	~ModulePhysics3D();

	bool Init(Document& document);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool Save(Document& document, FileWriteStream& fws);
	bool Load(Document& document);


	void CreateSphere(const float3 position, int radius);
	void CreateTriangle(const float3 &a_, const float3 &b_, const float3 &c_);
	std::list<float2> GetSphereCollisions();
	

	
	bool debug = false;


	std::vector<Sphere> spheres_list;

	
};

#endif