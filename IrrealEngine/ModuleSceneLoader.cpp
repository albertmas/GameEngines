#include "Application.h"
#include "ModuleSceneLoader.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleTextureLoader.h"
#include "ModuleMeshLoader.h"
#include "GameObject.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#include "mmgr/mmgr.h"


ModuleSceneLoader::ModuleSceneLoader(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneLoader::~ModuleSceneLoader()
{
}


bool ModuleSceneLoader::Init(Document& document)
{
	LOG("Loading FBX loader");
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = AssimpLog;
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleSceneLoader::CleanUp()
{
	LOG("Freeing all FBX loader elements");
	// detach log stream
	aiDetachAllLogStreams();

	RELEASE(ObjectBB);

	return true;
}

bool ModuleSceneLoader::ImportMesh(const char* full_path)
{
	bool ret = true;

	mesh_number = 0;

	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		ObjectBB = new AABB({ 0,0,0 }, { 0,0,0 });

		aiNode* rootNode = scene->mRootNode;
		LoadFile(full_path, scene, rootNode, nullptr);

		aiReleaseImport(scene);
	}
	else
	{
		LOG("WARNING, Error loading scene %s", full_path);
	}

	return ret;
}

GameObject* ModuleSceneLoader::LoadFile(const char* full_path, const aiScene* scene, aiNode* node, GameObject* parent)
{
	bool ret = true;

	if (node == nullptr)
		return false;

	GameObject* gameobject = nullptr;

	if (node->mMetaData != nullptr || parent == nullptr)
	{
		if (parent == nullptr)
			parent = App->scene->root;

		gameobject = new GameObject(parent, node->mName.C_Str());
		ComponentTransform* comp_trans = (ComponentTransform*)gameobject->CreateComponent(Component::TRANSFORMATION);

		aiVector3D position;
		aiQuaternion rotation;
		aiVector3D scaling;
		node->mTransformation.Decompose(scaling, rotation, position);
		float3 pos = { position.x, position.y, position.z };
		float3 scale = { scaling.x, scaling.y, scaling.z };
		Quat rot = Quat(rotation.x, rotation.y, rotation.z, rotation.w);

		if (parent == App->scene->root)
		{
			// Setting a counter for GameObjects
			parent_number++;
			gameobject->go_name = std::to_string(parent_number) + ": " + gameobject->go_name;
			App->scene->game_objects.push_back(gameobject);

			comp_trans->position = pos;
			comp_trans->rotation = rot;
			comp_trans->scale = scale;
		}

		for (int meshNum = 0; meshNum < node->mNumMeshes; meshNum++)
		{
			mesh_number++;
			LOG("\nLoading mesh %i -------", mesh_number);

			GameObject* gameobject_child = gameobject;
			if (node->mNumMeshes > 1)
			{
				std::string newName = "Untitled";
				if (scene->mMeshes[node->mMeshes[meshNum]]->mName.length > 0)
					newName = scene->mMeshes[node->mMeshes[meshNum]]->mName.C_Str();
				gameobject_child = new GameObject(gameobject, newName.c_str());
			}

			aiMesh* currentMesh = scene->mMeshes[node->mMeshes[meshNum]];
			FBXMesh* mesh = App->meshloader->ImportMesh(currentMesh);
			mesh->setMeshBuffer();

			aiMaterial* material = scene->mMaterials[currentMesh->mMaterialIndex];
			aiColor3D color(0.0f, 0.0f, 0.0f);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			/*mesh->color.Set(color.r, color.g, color.b);*/

			if (currentMesh->HasFaces())
			{
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
					newtexture->tex = false;
				}
				newtexture->color.Set(color.r, color.g, color.b);

				// Get info
				mesh->meshPath = full_path;
				mesh->meshName = currentMesh->mName.C_Str();
				mesh->meshNum = mesh_number;
				mesh->num_triangles = currentMesh->mNumFaces;
				mesh->bounding_box.SetNegativeInfinity();
				mesh->bounding_box.Enclose((float3*)currentMesh->mVertices, currentMesh->mNumVertices);
				ObjectBB->Enclose(mesh->bounding_box);

				mesh->meshPos = pos;
				mesh->meshRot = rot;
				mesh->meshScale = scale;

				// Set GO components
				ComponentTransform* child_trans = comp_trans;
				if (node->mNumMeshes > 1)
					child_trans = (ComponentTransform*)gameobject_child->CreateComponent(Component::TRANSFORMATION);
				child_trans->position = pos;
				child_trans->rotation = rot;
				child_trans->scale = scale;
				child_trans->matrix_local.Set(float4x4::FromTRS(pos, rot, scale));
				ComponentMesh* c_mesh = (ComponentMesh*)gameobject_child->CreateComponent(Component::MESH);
				c_mesh->SetMesh(mesh);
				if (newtexture)
				{
					ComponentTexture* c_tex = (ComponentTexture*)gameobject_child->CreateComponent(Component::TEXTURE);
					c_tex->texture = newtexture;

					c_mesh->SetCompTexture(c_tex);
				}

				gameobject_child->local_AABB.SetNegativeInfinity();
				gameobject_child->local_AABB.Enclose((float3*)currentMesh->mVertices, currentMesh->mNumVertices);
				gameobject_child->oriented_BB.SetNegativeInfinity();
				gameobject_child->oriented_BB.SetFrom(gameobject_child->local_AABB);

				if (node->mNumMeshes > 1)
				{
					gameobject->local_AABB.Enclose(gameobject_child->local_AABB);
					gameobject->oriented_BB.SetFrom(gameobject->local_AABB);
				}
			}
		}

		for (int i = 0; i < node->mNumChildren; i++)
		{
			GameObject* child = LoadFile(full_path, scene, node->mChildren[i], gameobject);
			if (child != nullptr)
			{
				gameobject->local_AABB.Enclose(child->local_AABB);
				gameobject->oriented_BB.SetFrom(gameobject->local_AABB);
			}
		}
	}
	else if (node->mNumChildren > 0)
	{
		for (int i = 0; i < node->mNumChildren; i++)
		{
			GameObject* child = LoadFile(full_path, scene, node->mChildren[i], parent);
			if (child != nullptr)
			{
				parent->local_AABB.Enclose(child->local_AABB);
				parent->oriented_BB.SetFrom(parent->local_AABB);
			}
		}
	}

	if (App->camera->first_time == false)
	{
		App->camera->FocusBox(*ObjectBB);
	}
	
	App->camera->first_time = false;
	
	return gameobject;
}


//void ModuleSceneLoader::ChangeTexure(const char* full_path)
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

