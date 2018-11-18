#include "Application.h"
#include "ModuleMeshLoader.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/mesh.h"

#include <fstream>

#include "mmgr/mmgr.h"


ModuleMeshLoader::ModuleMeshLoader(bool start_enabled) : Module(start_enabled)
{
}

ModuleMeshLoader::~ModuleMeshLoader()
{
}


FBXMesh* ModuleMeshLoader::ImportMesh(aiMesh* mesh, const char* name)
{
	FBXMesh* newMesh = new FBXMesh();
	if (name != "")
		newMesh->meshName = name;
	else if (mesh->mName.length > 0)
		newMesh->meshName = mesh->mName.C_Str();
	newMesh->meshUUID = pcg32_random_r(&App->rng);

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
				LOG("WARNING, geometry face with != 3 indices! This mesh won't be rendered.");
				newMesh->hasTriFaces = false;
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

	App->meshloader->meshes.push_back(newMesh);
	SaveMesh(newMesh);

	return newMesh;
}

bool ModuleMeshLoader::SaveMesh(FBXMesh* mesh)
{
	uint ranges[4] = { mesh->num_indices, mesh->num_vertices, mesh->num_normals, mesh->num_texCoords };
	uint size = sizeof(ranges) + sizeof(uint) * mesh->num_indices + sizeof(float) * mesh->num_vertices * 3 + sizeof(float) * mesh->num_normals * 3 + sizeof(float) * mesh->num_texCoords * 2;;
	size += sizeof(int);

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

	cursor += bytes; // Store hasTriFaces bool
	int tris[1] = { ((mesh->hasTriFaces) ? 1 : 0) };
	bytes = sizeof(int);
	memcpy(cursor, tris, bytes);

	// Save file
	std::string correctPath = MESHES_DIRECTORY + mesh->meshName + MESHES_EXTENSION;
	std::ofstream dataFile(correctPath, std::ofstream::binary);
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
	std::ifstream dataFile(correctPath, std::ifstream::binary);

	std::streamsize filesize = dataFile.tellg();
	dataFile.seekg(0, std::ios::end);
	filesize = dataFile.tellg() - filesize;
	dataFile.seekg(0, std::ios::beg);

	char* data = new char[filesize]; // Allocate

	if (dataFile.good())
	{
		dataFile.read(data, filesize);
		if (!dataFile)
			LOG("Error: Only  %i  could be read", dataFile.gcount());
		dataFile.close();
	}
	char* cursor = data;

	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	// Load ranges
	cursor += bytes;
	newMesh->num_indices = ranges[0];
	newMesh->num_vertices = ranges[1];
	newMesh->num_normals = ranges[2];
	newMesh->num_texCoords = ranges[3];

	// Load indices
	if (newMesh->num_indices > 0)
	{
		bytes = sizeof(uint) * newMesh->num_indices;
		newMesh->indices = new uint[newMesh->num_indices];
		memcpy(newMesh->indices, cursor, bytes);
		cursor += bytes;
	}

	// Load vertices
	if (newMesh->num_vertices > 0)
	{
		bytes = sizeof(float) * newMesh->num_vertices * 3;
		newMesh->vertices = new float[newMesh->num_vertices * 3];
		memcpy(newMesh->vertices, cursor, bytes);
		cursor += bytes;
	}

	// Load normals
	if (newMesh->num_normals > 0)
	{
		bytes = sizeof(float) * newMesh->num_normals * 3;
		newMesh->normals = new float[newMesh->num_normals * 3];
		memcpy(newMesh->normals, cursor, bytes);
		cursor += bytes;
	}

	// Load texture coords
	if (newMesh->num_texCoords > 0)
	{
		bytes = sizeof(float) * newMesh->num_texCoords * 2;
		newMesh->texCoords = new float[newMesh->num_texCoords * 2];
		memcpy(newMesh->texCoords, cursor, bytes);
		cursor += bytes;
	}

	// Load hasTriFaces bool
	int tris[1];
	bytes = sizeof(int);
	memcpy(tris, cursor, bytes);
	newMesh->hasTriFaces = tris[0];
	if (!newMesh->hasTriFaces)
		int a = 0;

	// Set AABB
	newMesh->bounding_box.SetNegativeInfinity();
	newMesh->bounding_box.Enclose((float3*)newMesh->vertices, newMesh->num_vertices);

	// Set name & path
	newMesh->meshName = name;
	newMesh->meshPath = correctPath;

	newMesh->setMeshBuffer();
	App->meshloader->meshes.push_back(newMesh);

	RELEASE_ARRAY(data);

	return newMesh;
}

FBXMesh::~FBXMesh()
{
	RELEASE_ARRAY(vertices);

	RELEASE_ARRAY(normals);

	RELEASE_ARRAY(texCoords);

	glDeleteBuffers(1, &id_indices);
	RELEASE_ARRAY(indices);
}

void FBXMesh::setMeshBuffer()
{
	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * num_vertices * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}