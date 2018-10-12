#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleRenderer3D.h"

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
			LOG("Loading mesh %i of %i -------", meshNum + 1, scene->mNumMeshes);

			FBXMesh* mesh = new FBXMesh();
			aiMesh* currentMesh = scene->mMeshes[meshNum];

			mesh->num_vertices = currentMesh->mNumVertices;
			mesh->vertices = new float[mesh->num_vertices * 3];
			memcpy(mesh->vertices, currentMesh->mVertices, sizeof(float) * mesh->num_vertices * 3);
			LOG("New mesh with %d vertices", mesh->num_vertices);

			mesh->num_normals = currentMesh->mNumVertices;
			mesh->normals = new float[mesh->num_normals * 3];
			memcpy(mesh->normals, currentMesh->mNormals, sizeof(float) * mesh->num_normals * 3);

			aiMaterial* material = scene->mMaterials[currentMesh->mMaterialIndex];
			aiColor3D color(0.f, 0.f, 0.f);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			mesh->color.x = color.r;
			mesh->color.y = color.g;
			mesh->color.z = color.b;

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
					// Searches for a texture with the same name in the /Textures folder
					std::string Path = full_path;
					for (int i = Path.size() - 1; i >= 0; i--)
						if (Path[i] == '/' | Path[i] == '\\')
							break;
						else
							Path.pop_back();
					Path += "Textures/";
					Path += path.C_Str();
					mesh->texture = LoadTexture(Path.c_str());
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

GLuint ModuleFBXLoader::LoadTexture(const char* full_path)
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
	}
	else
	{
		error = ilGetError();
		LOG("Image load failed - IL reports error: %s", iluErrorString(error));
		return -1;
	}

	ilDeleteImages(1, &imageID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	LOG("Texture creation successful.");

	return textureID;
}