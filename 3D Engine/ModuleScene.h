#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"
class ModuleScene :	public Module
{
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Init(Document& document);
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool Save(Document& document, FileWriteStream& fws);
	bool Load(Document& document);

	bool Draw()const;

private:

};

#endif