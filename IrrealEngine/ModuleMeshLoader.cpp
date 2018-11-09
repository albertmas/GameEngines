#include "Application.h"
#include "ModuleMeshLoader.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/mesh.h"

#include <fstream>


ModuleMeshLoader::ModuleMeshLoader(bool start_enabled) : Module(start_enabled)
{
}

ModuleMeshLoader::~ModuleMeshLoader()
{
}


//bool ModuleMeshLoader::Init(Document& document)
//{
//	return true;
//}

bool ModuleMeshLoader::ImportMesh(aiMesh* mesh)
{
	FBXMesh* newMesh = new FBXMesh();
	if (mesh->mName.length > 0)
		newMesh->meshName = mesh->mName.C_Str();

	// Vertices
	if (mesh->mNumVertices > 0)
	{
		newMesh->num_vertices = mesh->mNumVertices;
		newMesh->vertices = new float[newMesh->num_vertices * 3];
		memcpy(newMesh->vertices, mesh->mVertices, sizeof(float) * newMesh->num_vertices * 3);
		LOG("New mesh with %d vertices", newMesh->num_vertices);
	}
	// Normals
	if (mesh->HasNormals())
	{
		newMesh->num_normals = mesh->mNumVertices;
		newMesh->normals = new float[newMesh->num_normals * 3];
		memcpy(newMesh->normals, mesh->mNormals, sizeof(float) * newMesh->num_normals * 3);
	}
	// Indices
	if (mesh->HasFaces())
	{
		newMesh->num_indices = mesh->mNumFaces * 3;
		newMesh->indices = new uint[newMesh->num_indices]; // assume each face is a triangle

		bool verticeError = false;

		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
				verticeError = true;
			}
			else
			{
				memcpy(&newMesh->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
		/*if (!verticeError)
		{
			LOG("New mesh with %d indices", newMesh->num_indices);
			newMesh->setMeshBuffer();
			App->renderer3D->meshes.push_back(newMesh);
		}*/

		// Tex coords
		if (mesh->HasTextureCoords(0))
		{
			int c = 0;
			newMesh->texCoords = new float[newMesh->num_vertices * 2];
			for (uint num = 0; num < newMesh->num_vertices * 2; num += 2)
			{
				newMesh->texCoords[num] = mesh->mTextureCoords[0][c].x;
				newMesh->texCoords[num + 1] = mesh->mTextureCoords[0][c].y;
				c++;
			}
			newMesh->num_texCoords = mesh->mNumVertices;
		}
		else
			LOG("New mesh has no Texture Coordinates")
	}

	SaveMesh(newMesh);

	return true;
}

bool ModuleMeshLoader::SaveMesh(FBXMesh* mesh)
{
	uint ranges[4] = { mesh->num_indices, mesh->num_vertices, mesh->num_normals, mesh->num_texCoords };
	uint size = sizeof(ranges) + sizeof(uint) * mesh->num_indices + sizeof(float) * mesh->num_vertices * 3 + sizeof(float) * mesh->num_normals * 3 + sizeof(float) * mesh->num_texCoords * 2;;

	char* data = new char[size]; // Allocate
	char* cursor = data;

	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);

	cursor += bytes; // Store indices
	bytes = sizeof(uint) * mesh->num_indices;
	memcpy(cursor, mesh->indices, bytes);

	cursor += bytes; // Store vertices
	bytes = sizeof(float) * mesh->num_vertices * 3;
	memcpy(cursor, mesh->vertices, bytes);

	cursor += bytes; // Store normals
	bytes = sizeof(float) * mesh->num_normals * 3;
	memcpy(cursor, mesh->normals, bytes);

	cursor += bytes; // Store texture coords
	bytes = sizeof(float) * mesh->num_texCoords * 2;
	memcpy(cursor, mesh->texCoords, bytes);

	// Save file
	std::string correctPath = MESHES_DIRECTORY + mesh->meshName + MESHES_EXTENSION;
	std::ofstream dataFile(correctPath, std::fstream::out | std::fstream::binary);
	if (dataFile.is_open())
	{
		dataFile.write((const char*)data, size);
		dataFile.close();
	}

	RELEASE_ARRAY(data);

	return true;
}

FBXMesh* ModuleMeshLoader::LoadMesh(const char* name)
{
	FBXMesh* newMesh = new FBXMesh();
	std::string correctPath = MESHES_DIRECTORY;
	correctPath += name;
	correctPath += MESHES_EXTENSION;

	// Open file
	char* cursor = nullptr;
	std::ifstream dataFile(correctPath, std::fstream::in | std::fstream::binary);
	if (dataFile.is_open())
	{
		dataFile.read(cursor, sizeof(cursor));
		dataFile.close();
	}

	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	// Load ranges
	newMesh->num_indices = ranges[0];
	newMesh->num_vertices = ranges[1];
	newMesh->num_normals = ranges[2];
	newMesh->num_texCoords = ranges[3];

	// Load indices
	cursor += bytes;
	bytes = sizeof(uint) * newMesh->num_indices;
	newMesh->indices = new uint[newMesh->num_indices];
	memcpy(newMesh->indices, cursor, bytes);

	// Load vertices
	cursor += bytes;
	bytes = sizeof(float) * newMesh->num_vertices * 3;
	newMesh->vertices = new float[newMesh->num_vertices * 3];
	memcpy(newMesh->vertices, cursor, bytes);

	// Load normals
	cursor += bytes;
	bytes = sizeof(float) * newMesh->num_normals * 3;
	newMesh->normals = new float[newMesh->num_normals * 3];
	memcpy(newMesh->normals, cursor, bytes);

	// Load texture coords
	cursor += bytes;
	bytes = sizeof(float) * newMesh->num_texCoords * 2;
	newMesh->texCoords = new float[newMesh->num_texCoords * 2];
	memcpy(newMesh->texCoords, cursor, bytes);

	// Set AABB
	newMesh->bounding_box.SetNegativeInfinity();
	newMesh->bounding_box.Enclose((float3*)newMesh->vertices, newMesh->num_vertices);

	// Set name
	newMesh->meshName = name;

	newMesh->setMeshBuffer();
	App->renderer3D->meshes.push_back(newMesh);

	return newMesh;
}