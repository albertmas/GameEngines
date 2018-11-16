#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "PCG\pcg_basic.h"

using namespace rapidjson;

class GameObject;

class Component
{
public:

	Component()
	{}
	Component(GameObject* parent)
	{}
	virtual ~Component()
	{}
	
	virtual void Enable()
	{}
	virtual bool Update() { return true; }
	
	virtual void Disable()
	{}

	virtual void SetInspectorInfo()
	{}

	virtual bool Save(Document& document, FileWriteStream& fws) const
	{
		Document::AllocatorType& allocator = document.GetAllocator();
		Writer<FileWriteStream> writer(fws);
		return true;
	}
	virtual bool Load(Document& document)
	{
		return true;
	}

public:

	enum COMP_TYPE {
		NONE,
		MESH,
		TRANSFORMATION,
		TEXTURE,
		CAMERA
	};

public:

	bool active = true;
	GameObject* my_go = nullptr;
	COMP_TYPE type = NONE;
	uint UUID = 0;
};

#endif