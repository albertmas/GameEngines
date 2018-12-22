#ifndef _MODULE_AUDIO_H_
#define _MODULE_AUDIO_H_

#include "Module.h"
//#include "Include_Wwise.h"
#include "Wwise.h"


#define AUDIO_DIRECTORY "Library/Sounds/"
#define AUDIO_EXTENSION ".bnk"
#define DEFAULT_VOLUME 50

//class SoundBank;

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

	void LoadSoundBank(const char* path);

	//Game Objects
	Wwise::WwiseGameObject* CreateSoundObject(const char* name, math::float3 position);
	Wwise::WwiseGameObject* CreateListener(const char* name, math::float3 position);

	void SetVolume(const char* rtpc, float value);
	//void StopAllEvents();
	//void ImGuiDraw();

public:
	int volume = DEFAULT_VOLUME;
	bool muted = false;
	Wwise::WwiseGameObject* camera_listener = nullptr;
	std::list <Wwise::WwiseGameObject*> sound_obj;
	//SoundBank* soundbank = nullptr;

	//Listener* default_listener = nullptr;
	unsigned long listener_id = 1;
	unsigned long last_go_id = 100;
	bool listener_created = false;


};


#endif // _MODULE_AUDIO_H_