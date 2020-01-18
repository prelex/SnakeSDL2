#pragma once
#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

class Texture
{
public:
	Texture();
	~Texture();
	bool loadFromRenderedText(std::string textureText, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer);
	void free();
	void render(int x, int y, SDL_Renderer *renderer);
	int getWidth();
	int getHeight();
private:
	SDL_Texture *texture;
	int width;
	int height;
};