#include "Application.h"
#include "ModuleTextureLoader.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

#include <fstream>
#include <algorithm>


ModuleTextureLoader::ModuleTextureLoader(bool start_enabled) : Module(start_enabled)
{
}


ModuleTextureLoader::~ModuleTextureLoader()
{
}


bool ModuleTextureLoader::Init(Document& document)
{
	// DevIL
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return true;
}

bool ModuleTextureLoader::CleanUp()
{
	LOG("Freeing all Texture loader elements");

	for (std::list<Texture*>::iterator iter = textures.begin(); iter != textures.end(); iter++)
	{
		RELEASE((*iter));
	}
	textures.clear();

	// DevIL
	ilShutDown();

	return true;
}

bool ModuleTextureLoader::ImportTexture(const char* path, std::string& output_file)
{
	std::string texName = path;
	std::replace(texName.begin(), texName.end(), '\\', '/');
	uint lastSlash = texName.find_last_of('/');
	uint dot = texName.find_last_of('.');
	texName = texName.substr(lastSlash + 1, dot - lastSlash - 1);
	std::string correctPath;
	correctPath = TEXTURES_DIRECTORY + texName + TEXTURES_EXTENSION;
	// First it should check that the texture isn't already imported (use Physfs)

	ILuint imageID;
	ILboolean success;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	success = ilLoadImage(path);

	if (success)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		if (ilGetError() == IL_NO_ERROR)
		{
			ILuint size;
			ILubyte *data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
			size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
			if (size > 0) {
				data = new ILubyte[size]; // allocate data buffer
				if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
				{
					std::ofstream dataFile(correctPath, std::fstream::out | std::fstream::binary);
					dataFile.write((const char*)data, size);
					dataFile.close();
				}
				RELEASE_ARRAY(data);
			}
		}
	}
	else
	{
		ILenum error = ilGetError();
		LOG("Image load failed - IL reports error: %s", iluErrorString(error));
		return false;
	}

	ilDeleteImages(1, &imageID);

	output_file = correctPath;

	return true;
}

Texture* ModuleTextureLoader::LoadTexture(const char* full_path)
{
	Texture* newTex = new Texture();

	ILuint imageID;
	GLuint textureID;

	ILboolean success;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	success = ilLoadImage(full_path);

	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (!success)
		{
			error = ilGetError();
			LOG("Image conversion failed - IL reports error: %s", iluErrorString(error));
			return nullptr;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		// Set texture interpolation method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself

		newTex->width = ImageInfo.Width;
		newTex->height = ImageInfo.Height;
	}
	else
	{
		error = ilGetError();
		LOG("Image load failed - IL reports error: %s", iluErrorString(error));
		return 0;
	}

	ilDeleteImages(1, &imageID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	newTex->id = textureID;
	newTex->path = full_path;

	LOG("Texture creation successful.");

	return newTex;
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}
