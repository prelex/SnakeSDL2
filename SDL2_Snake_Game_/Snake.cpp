#include "Snake.h"
#include "ResourceManager.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int CELL_SIZE;
extern const int NUMBER_OF_CELLS_X;
extern const int NUMBER_OF_CELLS_Y;

Snake::Snake()
{
	segments.reserve(NUMBER_OF_CELLS_X*NUMBER_OF_CELLS_Y);
	Segment head = { CELL_SIZE, (SCREEN_WIDTH - CELL_SIZE) / 2, (SCREEN_HEIGHT - CELL_SIZE) / 2, NONE };
	segments.push_back(head);
}

void Snake::move()
{
	for (Segment& segment : segments)
	{
		switch (segment.direction)
		{
		case UP:
		{
			segment.y -= CELL_SIZE;
		}
		break;
		case DOWN:
		{
			segment.y += CELL_SIZE;
		}
		break;
		case LEFT:
		{
			segment.x -= CELL_SIZE;
		}
		break;
		case RIGHT:
		{
			segment.x += CELL_SIZE;
		}
		break;
		}
	}

	updateSnake();
}

void Snake::render(SDL_Renderer *renderer)
{
	// body
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	for (Segment &segment : segments)
	{
		SDL_Rect rect { segment.x, segment.y, segment.size, segment.size };
		SDL_RenderFillRect(renderer, &rect);
	}
	// outline
	SDL_SetRenderDrawColor(renderer, 20, 100, 0, 255);
	for (Segment &segment : segments)
	{
		SDL_Rect rect{ segment.x, segment.y, segment.size, segment.size };
		SDL_RenderDrawRect(renderer, &rect);
	}
}

void Snake::reset()
{
	segments.clear();
	*this = Snake();
}

void Snake::updateSnake()
{
	for (std::vector<Segment>::reverse_iterator iter = segments.rbegin(); iter < segments.rend() - 1; ++iter)
	{
		iter->direction = (iter + 1)->direction;
	}
}