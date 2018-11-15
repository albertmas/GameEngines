#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneLoader.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"

#include "mmgr/mmgr.h"


ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
}

ModuleScene::~ModuleScene()
{
}


bool ModuleScene::Init(Document& document)
{
	LOG("Loading Scene");
	LOG("Loading Plane");
	bool ret = true;

	App->camera->Move(vec3(0.0f, 10.0f, 0.0f));
	App->camera->LookAt(vec3(0.0f, 0.0f, 0.0f));

	return ret;
}

bool ModuleScene::Start()
{
	root = new GameObject(nullptr, "root");
	game_objects.push_back(root);
	ComponentTransform* root_trans = (ComponentTransform*)root->CreateComponent(Component::TRANSFORMATION);

	App->sceneloader->ImportMesh("Assets/street/Street environment_V01.fbx");

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	SetGlobalMatrix(root);

	return UPDATE_CONTINUE;
}
update_status ModuleScene::Update(float dt)
{
	Draw();

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	LOG("Freeing all Scene elements");
	
	return true;
}

bool ModuleScene::Save(Document& document, FileWriteStream& fws)
{
	Document::AllocatorType& allocator = document.GetAllocator();
	Writer<FileWriteStream> writer(fws);
	return true;
}

bool ModuleScene::Load(Document& document)
{
	return true;
}

void ModuleScene::Draw()
{
	for (int i = 0; i < game_objects.size(); i++)
	{
		if (game_objects[i] != root)
			game_objects[i]->Draw();
		/*if (BB && game_objects.size() > 1)
		{
			DrawBB((*iter)->bounding_box, { 1.0f, 0.0f, 0.0f });
		}*/
	}
}

GameObject* ModuleScene::CreateGameObject()
{
	GameObject* gameobject = new GameObject(root, "");
	game_objects.push_back(gameobject);

	return gameobject;
}

void ModuleScene::SetGlobalMatrix(GameObject* gameobject)
{
	ComponentTransform* compTrans = (ComponentTransform*)gameobject->GetComponent(Component::TRANSFORMATION);

	if (compTrans != nullptr)
	{
		if (gameobject->go_parent != nullptr)
			compTrans->matrix_global = ((ComponentTransform*)gameobject->go_parent->GetComponent(Component::TRANSFORMATION))->matrix_global * compTrans->matrix_local;

		for (int i = 0; i < gameobject->go_children.size(); i++)
		{
			SetGlobalMatrix(gameobject->go_children[i]);
		}
	}
}
