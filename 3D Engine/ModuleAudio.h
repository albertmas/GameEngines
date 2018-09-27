#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f
#define DEFAULT_MUSIC_VOLUME 128
#define DEFAULT_FX_VOLUME 128

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Init(Document& document);
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0, int channel = -1);

	bool Save(Document& document, FileWriteStream& fws);
	bool Load(Document& document);

private:

	Mix_Music*			music;
	std::list<Mix_Chunk*>	fx;
	int fx_volume = DEFAULT_FX_VOLUME;
	int music_volume = DEFAULT_MUSIC_VOLUME;
};

#endif // __ModuleAudio_H__