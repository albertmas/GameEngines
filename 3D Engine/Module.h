#pragma once

#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"
#include "PCG\pcg_basic.h"
#include "SDL/include/SDL.h"


class Module
{
private :
	bool enabled;

public:
	std::string name;
	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	/*virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}*/
};
