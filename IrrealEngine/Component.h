#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "PCG/pcg_basic.h"
#include "MathGeoLib/MathGeoLib.h"

using namespace rapidjson;

class GameObject;
class ComponentTransform;
class ComponentMesh;
class ComponentTexture;
class ComponentCamera;
class ComponentAudioSource;
class ComponentAudioListener;

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
	ComponentTransform* AsTransform()
	{
		return (ComponentTransform*)this;
	}
	ComponentMesh* AsMesh()
	{
		return (ComponentMesh*)this;
	}
	ComponentTexture* AsTexture()
	{
		return (ComponentTexture*)this;
	}
	ComponentCamera* AsCamera()
	{
		return (ComponentCamera*)this;
	}
	ComponentAudioSource* AsAudioSource()
	{
		return (ComponentAudioSource*)this;
	}
	ComponentAudioListener* AsAudioListener()
	{
		return (ComponentAudioListener*)this;
	}

public:

	enum COMP_TYPE {
		NONE,
		TRANSFORMATION,
		MESH,
		TEXTURE,
		CAMERA,
		AUDIOSOURCE,
		AUDIOLISTENER
	};

public:

	bool active = true;
	GameObject* my_go = nullptr;
	COMP_TYPE type = NONE;
	uint UUID = 0;
};

#endif