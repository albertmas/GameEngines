#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneLoader.h"
#include "ModuleInput.h"
#include "ModulePick.h"
#include "ModuleImGui.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"
#include "QuadTree.h"
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


	return ret;
}

bool ModuleScene::Start()
{
	root = new GameObject(nullptr, "root");
	game_objects.push_back(root);
	ComponentTransform* root_trans = (ComponentTransform*)root->CreateComponent(Component::TRANSFORMATION);

	App->camera->StartEditorCam();

	GameObject* new_cam = CreateCamera();
	game_objects.push_back(new_cam);
	

	if (new_cam->HasCam())
		App->camera->cams_list.push_back(new_cam);
	App->camera->SetCurrentCam(new_cam);
	App->camera->NewCamera();

	GlobalQuadTree = new Quadtree(AABB(float3(-90, -70, -90), float3(60, 50, 50)), 0);

	for (uint i = 0; i < game_objects.size(); i++)
	{
		game_objects[i]->go_static= true;
	}
	for (uint i = 0; i < game_objects.size(); i++)
	{
		if (game_objects[i]->IsActive())
			GlobalQuadTree->Insert(game_objects[i]);
	}

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
	if (App->imgui->mouse_ray)
		App->ray->DrawRay();
}

GameObject* ModuleScene::CreateGameObject()
{
	GameObject* gameobject = new GameObject(root, "");
	game_objects.push_back(gameobject);

	return gameobject;
}

void ModuleScene::DrawGOBoundingBoxes()
{

	for (uint i = 0; i < game_objects.size(); i++)
	{
		if (game_objects[i]->IsActive())
			game_objects[i]->RenderBoundingBox();


	}

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


GameObject* ModuleScene::CreateCamera()
{
	GameObject* main_camera_go = new GameObject(root, "Main Camera");
	ComponentCamera* cam_comp = new ComponentCamera();
	main_camera_go->PushComponent(cam_comp);
	cam_comp->cam->SetFarPlane(1000);
	return main_camera_go;
}
GameObject * ModuleScene::Closest_go(std::vector<GameObject*> gameobjects, LineSegment line)
{
	float3 closest_point;
	float closest_distance = 99999;
	GameObject* closest_go = nullptr;
	float distance;

	for (std::vector<GameObject*>::iterator it = gameobjects.begin(); it != gameobjects.end(); it++)
	{
		GameObject* go = (*it);

		ComponentMesh* mesh = go->GetComponentMesh();

		if (go->HasMesh())
		{
			float3 point = { 0,0,0 };

			if (mesh->Firstpoint(line, point, distance))
			{
				if (distance < closest_distance)
				{
					closest_distance = distance;
					closest_point = point;
					closest_go = go;
				}
			}
		}
	}
	return closest_go;
}


void ModuleScene::ClickSelect(LineSegment mouse_ray)
{
	std::vector<GameObject*> intersected_list;

	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		GameObject* go = (*it);

		if (!go->HasMesh())
		{
			continue;
		}

		bool collided = mouse_ray.Intersects(go->GetBB());

		if (collided)
		{
			intersected_list.push_back(go);
		}
	}


	GameObject* closestGo = Closest_go(intersected_list, mouse_ray);
	if (closestGo == nullptr)
		return;
	if (GetSelected() != nullptr)
		GetSelected()->SetSelected(false);
	closestGo->SetSelected(true);
}

GameObject * ModuleScene::GetSelected()
{
	GameObject* aux = nullptr;
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		if ((*it)->IsSelected() == true)
			aux = (*it);
	}

	return aux;
}


ImVec2 ModuleScene::GetPos() const
{
	return pos;
}

ImVec2 ModuleScene::GetSize() const
{
	return size;
}