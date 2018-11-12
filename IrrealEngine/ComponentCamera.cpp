#include "ComponentCamera.h"
#include "ModuleCamera3D.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "MathGeoLib/MathGeoLib.h"
#include "GameObject.h"


ComponentCamera::ComponentCamera()
{
	cam = new Camera();
}

bool ComponentCamera::Start()
{
	ComponentTransform* trans = (ComponentTransform*)my_go->GetComponent(COMP_TYPE::TRANSFORMATION);
	

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





