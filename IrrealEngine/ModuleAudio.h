#ifndef _MODULE_AUDIO_H_
#define _MODULE_AUDIO_H_

#include "Module.h"
//#include "rapidjson\rapidjson.h"
#include "Include_Wwise.h"
#include "Wwise.h"



#define DEFAULT_VOLUME 50

class SoundBank;

class ModuleAudio : public Module {

public:

	ModuleAudio(bool start_enabled = true);
	virtual ~ModuleAudio();
	bool Init(Document& document) override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	bool Save(Document& document, FileWriteStream& fws)const override;
	bool Load(Document& document) override;

	////Game Objects 
	////Wwise::SoundObject* CreateSoundObject(const char* name, math::float3 position);
	////Wwise::SoundObject* CreateListener(const char* name, math::float3 position);

	void SetVolume(const char* rtpc, float value);
	//void StopAllEvents();
	//void ImGuiDraw();

public:
	Wwise::SoundObject* camera_listener = nullptr;
	std::list <Wwise::SoundObject*> sound_obj;
	SoundBank* soundbank = nullptr;
	int volume = DEFAULT_VOLUME;
	bool muted = false;


};


#endif // _MODULE_AUDIO_H_