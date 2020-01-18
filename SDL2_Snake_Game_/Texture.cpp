#include "Texture.h"
#include <iostream>

Texture::Texture() : texture(nullptr), width(0), height(0) {}

Texture::~Texture()
{
	free();
}

bool Texture::loadFromRenderedText(std::string textureText, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer)
{
	free();

	SDL_Surface *textSurface = TTF_RenderText_Solid(font, textureText.c_str(), color);
	if (textSurface == nullptr)
		std::cout << "Failed to render text surface. SDL_ttf error: " << TTF_GetError() << std::endl;
	else
	{
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == nullptr)
			std::cout << "Failed to create texture from rendered surface. SDL_Error: " << SDL_GetError() << std::endl;
		else
		{
			width = textSurface->w;
			height = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}

	return texture != nullptr;
}

void Texture::free()
{
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
		width = 0;
		height = 0;
	}
}

void Texture::render(int x, int y, SDL_Renderer *renderer)
{
	SDL_Rect renderQuad = { x, y, width, height };
	SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
}

int Texture::getWidth() { return width; }
int Texture::getHeight() { return height; }