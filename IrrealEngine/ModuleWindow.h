#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(Document& document);
	bool CleanUp();

	bool Save(Document& document, FileWriteStream& fws);
	bool Load(Document& document);

	void SetTitle(const char* title);

	void SetWinWidth(int width);
	void SetWinHeight(int height);
	int SetFullscreen(bool fullscreen);
	void SetResizable(bool resizable);
	void SetBorderless(bool borderless);
	int SetFullDesktop(bool fulldesktop);

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	int width = 0;
	int height = 0;
};

#endif // __ModuleWindow_H__