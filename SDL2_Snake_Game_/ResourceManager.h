#pragma once
#include <map>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class ResourceManager
{
public:
	static std::map<std::string, Mix_Chunk*> SoundFX;
	static std::map<std::string, TTF_Font*> Fonts;

	ResourceManager() = delete;

	static Mix_Chunk* LoadChunk(std::string file, std::string name);
	static TTF_Font* LoadFont(std::string file, int pointSize, std::string name);

	static Mix_Chunk* GetChunk(std::string name);
	static TTF_Font* GetFont(std::string name);

	static void Clear();
};