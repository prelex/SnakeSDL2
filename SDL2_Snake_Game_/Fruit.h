#pragma once
#include <SDL.h>
#include "Snake.h"

class Fruit
{
	friend class Snake;
	friend class Game;
public:
	Fruit();
	void render(SDL_Renderer *renderer);
private:
	int size;
	int x, y;
};