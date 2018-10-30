#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

bool GameObject::Init()
{
	return true;
}

bool GameObject::CleanUp()
{
	return true;
}

void GameObject::Update()
{

}

Component* GameObject::CreateComponent(COMP_TYPE type)
{
	Component* ret;

	switch (type)
	{
	case TRANSFORMATION:
		ret = CreateComponentTransform();
		break;
	case MESH:
		ret = App->renderer3D->CreateComponentMesh();
		break;
	case MATERIAL:
		ret = App->renderer3D->CreateComponentTexture();
		break;

	}

	return ret;
}

ComponentTransform* GameObject::CreateComponentTransform()
{
	ComponentTransform* ret;

	return ret;
}