#ifndef _MODULE_RENDERER_
#define _MODULE_RENDERER_

#include "Module.h"
#include "Light.h"
#include "glmath.h"
#include "Mesh.h"

#define MAX_LIGHTS 8
#define CHECKERS_WIDTH 100
#define CHECKERS_HEIGHT 100

struct FBXMesh
{
	~FBXMesh();

	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;
	

	uint id_normal = 0;
	uint num_normals = 0;
	float* normals = nullptr;
	

	vec3 color = { 1, 1, 1 };

	//uint texture = 0;
	uint num_texCoords = 0;
	float* texCoords = nullptr;

	AABB bounding_box;
	
	void setMeshBuffer();
	void Draw();

	// Mesh info
	std::string meshName = "Untitled";
	std::string meshPath = "";
	uint meshNum = 0;
	float3 meshPos = { 0, 0, 0 };
	Quat meshRot = { 0, 0, 0, 0 };
	float3 meshScale = { 0, 0, 0 };
	uint num_triangles = 0;
	bool hasTriFaces = true;
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(Document& document);
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool Save(Document& document, FileWriteStream& fws);
	bool Load(Document& document);


	void OnResize(int width, int height);
	void Info_init_Console();
	void FunctionsRender();
	void CreateCube();
	void DrawBB(AABB bounding_box, float3 color)const;
	void DrawBB(OBB bounding_box, float3 color)const;
	

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

	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

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
	std::list<FBXMesh*> meshes;

	uint tex_buff_id = 0;
};

#endif