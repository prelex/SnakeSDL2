#pragma once
#include <vector>
#include <SDL.h>
#include "Fruit.h"
#include "Game.h"

enum Direction {NONE, UP, DOWN, LEFT, RIGHT};

struct Segment
{
	int size;
	int x, y;
	Direction direction;
};

class Snake
{
	friend class Fruit;
	friend class Game;
public:
	Snake();
	void move();
	void render(SDL_Renderer *renderer);
private:
	std::vector<Segment> segments;
	void reset();
	void updateSnake();
};
