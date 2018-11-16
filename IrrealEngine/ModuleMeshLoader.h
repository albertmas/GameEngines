#ifndef __ModuleMeshLoader_H__
#define __ModuleMeshLoader_H__

#include "Module.h"
#include "glmath.h"

#define MESHES_DIRECTORY "Library/Meshes/"
#define MESHES_EXTENSION ".IrrealMesh"


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

class FBXMesh;
class aiMesh;

class ModuleMeshLoader : public Module
{
public:
	ModuleMeshLoader(bool start_enabled = true);
	~ModuleMeshLoader();

	//bool Init(Document& document);
	//bool CleanUp();

	FBXMesh* ImportMesh(aiMesh* mesh);
	bool SaveMesh(FBXMesh* mesh);
	FBXMesh* LoadMesh(const char* name);

public:
	std::list<FBXMesh*> meshes;
};

#endif