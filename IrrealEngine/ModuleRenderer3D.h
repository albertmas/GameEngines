#ifndef _MODULE_RENDERER_
#define _MODULE_RENDERER_

#include "Module.h"
#include "Light.h"
#include "glmath.h"
#include "Mesh.h"

#define MAX_LIGHTS 8
#define CHECKERS_WIDTH 100
#define CHECKERS_HEIGHT 100


class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D() override;

	bool Init(Document& document) override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	bool Save(Document& document, FileWriteStream& fws)const override;
	bool Load(Document& document) override;


	void OnResize(int width, int height);
	void Info_init_Console();
	void FunctionsRender();
	void CreateCube();
	void DrawBB(AABB bounding_box, float3 color)const;
	void DrawBB(OBB bounding_box, float3 color)const;
	void DrawFrustum(Frustum frustum);

	void Active_Wireframe(bool active);
	void Active_Depth(bool active);
	void Active_Cull(bool active);
	void Active_Light(bool active);
	void Active_ColorMat(bool active);
	void Active_Texture2D(bool active);
	void Active_Normals(bool active);
	

	char* GetGraphicsVendor();
	char* GetGraphicsModel();

public:

	

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	/*float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;*/

	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

public:
	/*Mesh VertexAndIndexCube;
	Mesh VertexArrayCube;
	Mesh VertexSphere;*/

public:

	bool Wireframe = false;
	bool GetWireframe()const { return Wireframe; }

	bool Depth_Test = false;
	bool GetDepth() const { return Depth_Test; }

	bool Cull_Face = true;
	bool GetCullFace() const { return Cull_Face; }

	bool Lighting = false;
	bool GetLight() const { return Lighting; }

	bool Color_Material = true;
	bool GetColor() const { return Color_Material; }

	bool Texture_2D = true;
	bool GetTexture2D() const { return Texture_2D; }

	bool Normals = false;
	bool GetNormals() const { return Normals; }
	void SetNormals(bool active) { Normals = active; }
	
	bool BB = false;
	bool GetBB() const { return BB; }
	

	void CreatePlane();
	bool plane = true;

	void CreateAxis();
	bool axis = true;

	/*bool Cube = true;
	bool Sphere = true;*/

public:
	uint tex_buff_id = 0;
};

#endif