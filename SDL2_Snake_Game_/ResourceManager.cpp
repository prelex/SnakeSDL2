#include "ResourceManager.h"
#include <iostream>

std::map<std::string, Mix_Chunk*> ResourceManager::SoundFX;
std::map<std::string, TTF_Font*> ResourceManager::Fonts;

Mix_Chunk* ResourceManager::LoadChunk(std::string file, std::string name)
{
	SoundFX[name] = Mix_LoadWAV(file.c_str());
	return SoundFX[name];
}

TTF_Font* ResourceManager::LoadFont(std::string file, int pointSize, std::string name)
{
	Fonts[name] = TTF_OpenFont(file.c_str(), pointSize);
	return Fonts[name];
}

void ResourceManager::Clear()
{
	for (auto it = SoundFX.begin(); it != SoundFX.end(); ++it)
	{
		Mix_FreeChunk(it->second);
		SoundFX.erase(it);
	}
}

Mix_Chunk* ResourceManager::GetChunk(std::string name)
{
	return SoundFX[name];
}

TTF_Font* ResourceManager::GetFont(std::string name)
{
	return Fonts[name];
}