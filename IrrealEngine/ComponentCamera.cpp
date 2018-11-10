#include "ComponentCamera.h"
#include "ModuleCamera3D.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "MathGeoLib/MathGeoLib.h"
#include "GameObject.h"



ComponentCamera::ComponentCamera(GameObject* gameobject)
{
	my_go = gameobject;
	type = CAMERA;
}

ComponentCamera::ComponentCamera()
{

}

bool ComponentCamera::Start()
{
	


	return true;
}

bool ComponentCamera::Update()
{
	return true;
}


ComponentCamera::~ComponentCamera()
{
}

Camera * ComponentCamera::GetCamera()
{
	return cam;
}

void ComponentCamera::UpdatePos()
{

}





