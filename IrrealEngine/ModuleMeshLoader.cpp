#include "Application.h"
#include "ModuleMeshLoader.h"
#include "ModuleRenderer3D.h"


ModuleMeshLoader::ModuleMeshLoader(bool start_enabled) : Module(start_enabled)
{
}

ModuleMeshLoader::~ModuleMeshLoader()
{
}


//bool ModuleMeshLoader::Init(Document& document)
//{
//	return true;
//}

bool ModuleMeshLoader::ImportMesh(aiMesh* mesh)
{


	return true;
}

FBXMesh* ModuleMeshLoader::LoadMesh(const char* full_path)
{
	FBXMesh* newMesh = nullptr;



	return newMesh;
}