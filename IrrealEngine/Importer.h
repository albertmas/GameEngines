#ifndef _IMPORTER_H_
#define _IMPORTER_H_

#include "Module.h"
#include "Assimp.h"
#include "Open_GL.h"
#include "ModuleScene.h"


class Importer : public Module
{
public:
	Importer(bool start_enabled);
	~Importer();

	bool Init();
	bool CleanUp();

	bool Import(const std::string &full_path);
	void LoadMesh(const aiMesh* mesh);


};







#endif _IMPORTER_H_