#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneLoader.h"
#include "ModuleInput.h"
#include  "Component.h"
#include  "ComponentTransform.h"
#include  "ComponentMesh.h"
#include  "ComponentTexture.h"
#include "ComponentCamera.h"





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


	return ret;
}

bool ModuleScene::Start()
{
	root = new GameObject(nullptr, "root");
	root->go_static = true;
	game_objects.push_back(root);
	ComponentTransform* root_trans = (ComponentTransform*)root->CreateComponent(COMP_TYPE::TRANSFORMATION);


	App->camera->StartEditorCam();
	
	GameObject* new_cam = CreateCamera();
	game_objects.push_back(new_cam);
	

	if (new_cam->HasCam())
		App->camera->cams_list.push_back(new_cam);
	App->camera->SetCurrentCam(new_cam);
	App->camera->NewCamera();

	App->sceneloader->ImportMesh("Assets/BakerHouse/BakerHouse.fbx");
	
	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
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

		if (!game_objects[i]->IsStatic() && game_objects[i]->HasMesh())
		{
			//Check In Frustum
			if (App->camera->GetCurrentCam()->IsGameObjectInFrustum(game_objects[i]->GetBB(), game_objects[i]->comp_transform->matrix_local.TranslatePart()))
				game_objects[i]->Draw();
		}
		else if (!game_objects[i]->IsRoot())
			game_objects[i]->Draw();

		if (App->camera->GetCurrentCam()->frustrum_draw)
			App->camera->GetCurrentCam()->DrawFrustum();
		}
	
}

GameObject* ModuleScene::CreateGameObject()
{
	GameObject* gameobject = new GameObject(root, "");
	game_objects.push_back(gameobject);

	return gameobject;
}

GameObject* ModuleScene::CreateCamera()
{
	GameObject* main_camera_go = new GameObject();
	main_camera_go->SetName("Main Camera");
	ComponentCamera* cam_comp = new ComponentCamera();
	main_camera_go->PushComponent(cam_comp);
	cam_comp->cam->SetFarPlane(1000);
	return main_camera_go;
}