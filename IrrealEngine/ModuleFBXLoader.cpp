#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"


#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"
#include "DevIL\include\ilut.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")




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
	stream.callback = AssimpLog;
	aiAttachLogStream(&stream);

	// DevIL
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	
	
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
	ilShutDown();

	RELEASE(ObjectBB);

	return true;
}

bool ModuleFBXLoader::ImportMesh(const char* full_path)
{
	bool ret = true;

	mesh_number = 0;

	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* rootNode = scene->mRootNode;
		ObjectBB = new AABB({ 0,0,0 }, { 0,0,0 });

		LoadFile(full_path, scene, rootNode);

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading mesh scene %s", full_path);
	}

	return ret;
}

bool ModuleFBXLoader::LoadFile(const char* full_path, const aiScene* scene, aiNode* node)
{
	bool ret = true;

	for (int i = 0; i < node->mNumChildren; i++)
		LoadFile(full_path, scene, node->mChildren[i]);
	
	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scaling;
	node->mTransformation.Decompose(scaling, rotation, position);

	
	for (int meshNum = 0; meshNum < node->mNumMeshes; meshNum++)
	{
		mesh_number++;
		LOG("\nLoading mesh %i of %i -------", mesh_number, scene->mNumMeshes);

		FBXMesh* mesh = new FBXMesh();
		aiMesh* currentMesh = scene->mMeshes[node->mMeshes[meshNum]];

		mesh->num_vertices = currentMesh->mNumVertices;
		mesh->vertices = new float3[mesh->num_vertices * 3];
		memcpy(mesh->vertices, currentMesh->mVertices, sizeof(float) * mesh->num_vertices * 3);
		LOG("New mesh with %d vertices", mesh->num_vertices);

		mesh->num_normals = currentMesh->mNumVertices;
		mesh->normals = new float3[mesh->num_normals * 3];
		memcpy(mesh->normals, currentMesh->mNormals, sizeof(float) * mesh->num_normals * 3);

		aiMaterial* material = scene->mMaterials[currentMesh->mMaterialIndex];
		aiColor3D color(0.0f, 0.0f, 0.0f);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mesh->color.Set(color.r, color.g, color.b);
			
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
				LOG("New mesh with %d indices", mesh->num_indices);
				mesh->setMeshBuffer();
				App->renderer3D->meshes.push_back(mesh);
			}

			// Searching Texture
			aiString path;
			aiReturn error = material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);

			if (error == aiReturn::aiReturn_SUCCESS)
			{
				// Searches for the texture specified in the .fbx file
				std::string Path = full_path;
				for (int i = Path.size() - 1; i >= 0; i--)
					if (Path[i] == '/' | Path[i] == '\\')
						break;
					else
						Path.pop_back();
				Path += path.C_Str();
				mesh->texture = LoadTexture(Path.c_str(), mesh->texWidth, mesh->texHeight);
				mesh->texPath = Path.c_str();
			}
			else
				LOG("Couldn't load the default texture from .fbx file");

			if (currentMesh->HasTextureCoords(0))
			{
				int c = 0;
				mesh->texCoords = new float[mesh->num_vertices * 2];
				for (uint num = 0; num < mesh->num_vertices * 2; num += 2)
				{
					mesh->texCoords[num] = currentMesh->mTextureCoords[0][c].x;
					mesh->texCoords[num + 1] = currentMesh->mTextureCoords[0][c].y;
					c++;
				}
			}
				
			// Get info
			mesh->meshPath = full_path;
			mesh->meshName = currentMesh->mName.C_Str();
			mesh->meshNum = mesh_number;
			mesh->num_triangles = currentMesh->mNumFaces;
			mesh->bounding_box.SetNegativeInfinity();
			mesh->bounding_box.Enclose((float3*)currentMesh->mVertices, currentMesh->mNumVertices);
			ObjectBB->Enclose(mesh->bounding_box);

			mesh->meshPos.Set(position.x, position.y, position.z);
			mesh->meshRot.Set(rotation.x, rotation.y, rotation.z, rotation.w);
			mesh->meshScale.Set(scaling.x, scaling.y, scaling.z);
		}
	}

	if (App->camera->first_time == false)
	{
		App->camera->FocusBox(*ObjectBB);
	}
	
	App->camera->first_time = false;
	
	return ret;
}

GLuint ModuleFBXLoader::LoadTexture(const char* full_path, uint &width, uint &height)
{
	ILuint imageID;
	GLuint textureID;

	ILboolean success;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	success = ilLoadImage(full_path);

	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (!success)
		{
			error = ilGetError();
			LOG("Image conversion failed - IL reports error: %s", iluErrorString(error));
			return -1;
		}
		
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		// Set texture interpolation method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself

		width = ImageInfo.Width;
		height = ImageInfo.Height;
	}
	else
	{
		error = ilGetError();
		LOG("Image load failed - IL reports error: %s", iluErrorString(error));
		return 0;
	}

	ilDeleteImages(1, &imageID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	LOG("Texture creation successful.");

	return textureID;
}

void ModuleFBXLoader::ChangeTexure(const char* full_path)
{
	LOG("Changing Textures-------")
	for (std::list<FBXMesh*>::iterator iter = App->renderer3D->meshes.begin(); iter != App->renderer3D->meshes.end(); iter++)
	{
		(*iter)->texture = LoadTexture(full_path, (*iter)->texWidth, (*iter)->texHeight);
		(*iter)->texPath = full_path;
	}
}

void AssimpLog(const char* str, char* userData)
{
	std::string msg = str;
	msg.pop_back();
	msg.pop_back();

	LOG("%s", msg.c_str());
}

