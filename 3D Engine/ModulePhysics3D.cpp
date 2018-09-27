#include "Application.h"
#include "ModulePhysics3D.h"


ModulePhysics3D::ModulePhysics3D(bool start_enabled) : Module(start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init(Document& document)
{
	LOG("Creating 3D Physics simulation");
	bool ret = true;

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	

	return true;
}

bool ModulePhysics3D::Save(Document& document, FileWriteStream& fws)
{
	bool ret = true;

	return ret;
}

bool ModulePhysics3D::Load(Document& document)
{
	bool ret = true;

	return ret;
}


void ModulePhysics3D::CreateSphere(const float3 position, int radius)
{
	Sphere new_sphere;
	new_sphere.pos = position;
	new_sphere.r = radius;
	spheres_list.push_back(new_sphere);
}

void ModulePhysics3D::CreateTriangle(const float3 &a_, const float3 &b_, const float3 &c_)
{
	Triangle new_triangle;
	new_triangle;
	new_triangle.a = a_;
	new_triangle.b = b_;
	new_triangle.c = c_;

}

std::list<float2> ModulePhysics3D::GetSphereCollisions()
{
	int listener = 0;
	int candidate = 0;

	std::list<float2> collisions_list;

	for (listener; listener < spheres_list.size(); listener++)
	{
		bool collided = false;
		for (candidate; candidate < spheres_list.size(); candidate++)
		{
			if (listener == candidate)
				continue;
			collided = spheres_list[listener].Intersects(spheres_list[candidate]);
			if (collided)
			{
				collisions_list.push_back({ (float)listener,(float)candidate });
				
			}
		}
		candidate = 0;
	}
	return collisions_list;
}
