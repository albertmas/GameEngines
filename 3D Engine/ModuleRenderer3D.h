#pragma once
#include "Module.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(Document& document);
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool Save(Document& document, FileWriteStream& fws);
	bool Load(Document& document);


	void OnResize(int width, int height);
	void Info_init_Console();
	void FunctionsRender();

	char* GetGraphicsVendor();
	char* GetGraphicsModel();

public:

	bool Wireframe = false;
	bool Depth_Test = false;
	bool Cull_Face = false;
	bool Lighting = false;
	bool Color_Material = false;
	bool Texture_2D = false;
	bool debug_draw = false;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	
};