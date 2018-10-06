#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleFBXLoader::ModuleFBXLoader(bool start_enabled) : Module(start_enabled)
{
}

ModuleFBXLoader::~ModuleFBXLoader()
{
}


bool ModuleFBXLoader::Init(Document& document)
{
	LOG("Loading FBX loader");
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	
	return true;
}

update_status ModuleFBXLoader::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleFBXLoader::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleFBXLoader::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleFBXLoader::CleanUp()
{
	LOG("Freeing all FBX loader elements");
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

bool ModuleFBXLoader::LoadFile(const char* full_path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality); // It's important to choose a good flag
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int meshNum = 0; meshNum < scene->mNumMeshes; meshNum++)
		{
			FBXMesh* mesh = new FBXMesh();
			aiMesh* currentMesh = scene->mMeshes[meshNum];

			mesh->num_vertices = currentMesh->mNumVertices;
			mesh->vertices = new float[mesh->num_vertices * 3];
			memcpy(mesh->vertices, currentMesh->mVertices, sizeof(float) * mesh->num_vertices * 3);
			LOG("New mesh with %d vertices", mesh->num_vertices);

			mesh->num_normals = currentMesh->mNumVertices;
			mesh->normals = new float[mesh->num_normals * 3];
			memcpy(mesh->normals, currentMesh->mNormals, sizeof(float) * mesh->num_normals * 3);
			LOG("New mesh with %d normals", mesh->num_normals);

			if (currentMesh->HasFaces())
			{
				mesh->num_indices = currentMesh->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices]; // assume each face is a triangle

				bool verticeError = false;

				for (uint i = 0; i < currentMesh->mNumFaces; ++i)
				{
					if (currentMesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
						verticeError = true;
					}
					else
					{
						memcpy(&mesh->indices[i * 3], currentMesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
				if (!verticeError)
				{
					App->renderer3D->setMesh(mesh);
				}
			}
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", full_path);
		ret = false;
	}

	return ret;
}