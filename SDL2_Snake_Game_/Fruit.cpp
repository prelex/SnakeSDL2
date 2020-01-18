#include "Fruit.h"
#include <random>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int CELL_SIZE;
extern const int NUMBER_OF_CELLS_X;
extern const int NUMBER_OF_CELLS_Y;

Fruit::Fruit()
{
	std::uniform_int_distribution<unsigned> ux(0, NUMBER_OF_CELLS_X - 1), uy(0, NUMBER_OF_CELLS_Y - 1);
	std::default_random_engine e;

	int middleX = (SCREEN_WIDTH - CELL_SIZE) / 2;
	int middleY = (SCREEN_HEIGHT - CELL_SIZE) / 2;

	bool validPos;
	do
	{
		validPos = true;
		x = ux(e) * CELL_SIZE;
		y = uy(e) * CELL_SIZE;

		if (x == middleX && y == middleY)
			validPos = false;
	} while (!validPos);

	size = CELL_SIZE;
}

void Fruit::render(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect rect{ x, y, size, size };
	SDL_RenderFillRect(renderer, &rect);
}