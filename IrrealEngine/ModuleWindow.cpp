#include "Application.h"
#include "ModuleWindow.h"

#include "mmgr/mmgr.h"


ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(Document& document)
{
	LOG("Init SDL window & surface");
	bool ret = true;
	
	assert(document.IsObject());
	assert(document["window"].IsString()); // Should load defaults here

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = GetSystemMetrics(SM_CXSCREEN) * SCREEN_SIZE * 0.9f;
		height = GetSystemMetrics(SM_CYSCREEN) * SCREEN_SIZE * 0.9f;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

bool ModuleWindow::Save(Document& document, FileWriteStream& fws)
{
	bool ret = true;

	return ret;
}

bool ModuleWindow::Load(Document& document)
{
	bool ret = true;

	return ret;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

int ModuleWindow::SetFullscreen(bool fullscreen)
{
	int ret = -1;
	if (fullscreen)
	{
		ret = SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		ret = SDL_SetWindowFullscreen(window, 0);
	}
	return ret;
}

void ModuleWindow::SetResizable(bool resizable)
{
	if (resizable)
	{
		SDL_SetWindowResizable(window, SDL_TRUE);
	}
	else
	{
		SDL_SetWindowResizable(window, SDL_FALSE);
	}
}

void ModuleWindow::SetBorderless(bool borderless)
{
	if (borderless)
	{
		SDL_SetWindowBordered(window, SDL_FALSE);
	}
	else
	{
		SDL_SetWindowBordered(window, SDL_TRUE);
	}
}

int ModuleWindow::SetFullDesktop(bool fulldesktop)
{
	int ret = -1;

	if (fulldesktop)
	{
		ret = SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		ret = SDL_SetWindowFullscreen(window, 0);
	}

	return ret;
}

void ModuleWindow::SetWinWidth(int width_)
{
	width = width_;
	SDL_SetWindowSize(window, width, height);
}

void ModuleWindow::SetWinHeight(int height_)
{
	height = height_;
	SDL_SetWindowSize(window, width, height);
}