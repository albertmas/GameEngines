#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleTextureLoader.h"
#include "GameObject.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"


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
		LoadFile(full_path, scene, rootNode, nullptr);

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading mesh scene %s", full_path);
	}

	return ret;
}

bool ModuleFBXLoader::LoadFile(const char* full_path, const aiScene* scene, aiNode* node, GameObject* parent)
{
	bool ret = true;

	if (node == nullptr)
		return false;

	if (parent == nullptr)
		parent = App->scene->root;

	GameObject* gameobject = new GameObject(parent, node->mName.C_Str());

	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scaling;
	node->mTransformation.Decompose(scaling, rotation, position);

	if (parent == App->scene->root)
	{
		// Setting a counter for GameObjects
		parent_number++;
		gameobject->go_name = std::to_string(parent_number) + ": " + gameobject->go_name;
		App->scene->game_objects.push_back(gameobject);

		ComponentTransform* comp_trans = (ComponentTransform*)gameobject->CreateComponent(Component::TRANSFORMATION);
		comp_trans->position.Set(position.x, position.y, position.z);	
		comp_trans->rotation.Set(rotation.x, rotation.y, rotation.z, rotation.w);
		comp_trans->scale.Set(scaling.x, scaling.y, scaling.z);
	}

	for (int i = 0; i < node->mNumChildren; i++)
		LoadFile(full_path, scene, node->mChildren[i], gameobject);
	
		
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
		/*mesh->color.Set(color.r, color.g, color.b);*/
			
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
			Texture* newtexture = nullptr;

			if (error == aiReturn::aiReturn_SUCCESS)
			{
				// Searches for the texture specified in the .fbx file
				std::string correctPath = full_path;
				for (int i = correctPath.size() - 1; i >= 0; i--)
					if (correctPath[i] == '/' | correctPath[i] == '\\')
						break;
					else
						correctPath.pop_back();
				correctPath += path.C_Str();
				if (App->texloader->ImportTexture(correctPath.c_str(), correctPath))
					newtexture = App->texloader->LoadTexture(correctPath.c_str());
				//mesh->texPath = correctPath.c_str();
			}
			else
				LOG("Couldn't load the default texture from .fbx file");

			if (newtexture == nullptr)
			{
				newtexture = new Texture();
				newtexture->color.Set(color.r, color.g, color.b);
			}

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

			// Set GO components
			ComponentTransform* c_trans = (ComponentTransform*)gameobject->CreateComponent(Component::TRANSFORMATION);
			c_trans->position.Set(position.x, position.y, position.z);
			c_trans->rotation.Set(rotation.x, rotation.y, rotation.z, rotation.w);
			c_trans->scale.Set(scaling.x, scaling.y, scaling.z);
			ComponentMesh* c_mesh = (ComponentMesh*)gameobject->CreateComponent(Component::MESH);
			c_mesh->SetMesh(mesh);
			if (newtexture)
			{
				ComponentTexture* c_tex = (ComponentTexture*)gameobject->CreateComponent(Component::TEXTURE);
				c_tex->texture = newtexture;

				c_mesh->SetCompTexture(c_tex);
			}
		}
	}

	if (App->camera->first_time == false)
	{
		App->camera->FocusBox(*ObjectBB);
	}
	
	App->camera->first_time = false;
	
	return ret;
}


//void ModuleFBXLoader::ChangeTexure(const char* full_path)
//{
//	LOG("Changing Textures-------")
//	for (std::list<FBXMesh*>::iterator iter = App->renderer3D->meshes.begin(); iter != App->renderer3D->meshes.end(); iter++)
//	{
//		(*iter)->texture = App->texloader->LoadTexture(full_path);
//		(*iter)->texPath = full_path;
//	}
//}

void AssimpLog(const char* str, char* userData)
{
	std::string msg = str;
	msg.pop_back();
	msg.pop_back();

	LOG("%s", msg.c_str());
}

