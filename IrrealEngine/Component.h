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

	virtual Value Save(Document::AllocatorType& allocator) const
	{
		Value comp;
		return comp;
	}
	virtual bool Load(Document& document)
	{
		return true;
	}

public:

	enum COMP_TYPE {
		NONE,
		TRANSFORMATION,
		MESH,
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