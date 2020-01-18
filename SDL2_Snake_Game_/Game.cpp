#include "Game.h"
#include "Snake.h"
#include "Fruit.h"
#include "ResourceManager.h"
#include <iostream>
#include <random>
#include <memory>

extern const int CELL_SIZE;
extern const int NUMBER_OF_CELLS_X, NUMBER_OF_CELLS_Y;
extern SDL_Renderer *renderer;

std::uniform_int_distribution<unsigned> ux(0, NUMBER_OF_CELLS_X - 1), uy(0, NUMBER_OF_CELLS_Y - 1);
std::default_random_engine e;

std::unique_ptr<Snake> snake = nullptr;
std::unique_ptr<Fruit> fruit = nullptr;

Game::Game(int width, int height) : Width(width), Height(height), Score(0), HiScore(0), State(GAME_ACTIVE), ScoreText(), ScoreTextTexture() {}

bool Game::Init()
{
	bool success = true;

	// Load sound fx
	if (ResourceManager::LoadChunk("Sounds/eat_fruit.wav", "eat_fruit") == nullptr)
	{
		std::cout << "Failed to load eat fruit sound effect." << std::endl;
		success = false;
	}
	if (ResourceManager::LoadChunk("Sounds/game_over.wav", "game_over") == nullptr)
	{
		std::cout << "Failed to load game over sound effect." << std::endl;
		success = false;
	}
	if (ResourceManager::LoadChunk("Sounds/game_won.wav", "game_won") == nullptr)
	{
		std::cout << "Failed to load game won sound effect." << std::endl;
		success = false;
	}
	// Load font
	if (ResourceManager::LoadFont("Fonts/Prototype.ttf", 18, "score_font") == nullptr)
	{
		std::cout << "Failed to load font." << std::endl;
		success = false;
	}
	
	// Initialise game objects
	snake = std::make_unique<Snake>();
	fruit = std::make_unique<Fruit>();

	return success;
}

void Game::ProcessInput(SDL_Event &e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
		{
			if (snake->segments.size() > 1)
			{
				if (snake->segments[0].direction != DOWN)
					snake->segments[0].direction = UP;
			}
			else
				snake->segments[0].direction = UP;
		}
		break;
		case SDLK_DOWN:
		{
			if (snake->segments.size() > 1)
			{
				if (snake->segments[0].direction != UP)
					snake->segments[0].direction = DOWN;
			}
			else
				snake->segments[0].direction = DOWN;
		}
		break;
		case SDLK_LEFT:
		{
			if (snake->segments.size() > 1)
			{
				if (snake->segments[0].direction != RIGHT)
					snake->segments[0].direction = LEFT;
			}
			else
				snake->segments[0].direction = LEFT;
		}
		break;
		case SDLK_RIGHT:
		{
			if (snake->segments.size() > 1)
			{
				if (snake->segments[0].direction != LEFT)
					snake->segments[0].direction = RIGHT;
			}
			else
				snake->segments[0].direction = RIGHT;
		}
		break;
		}
	}
}

void Game::Update()
{
	if (State == GAME_ACTIVE)
	{
		// Update snake position
		snake->move();

		// Check for collisions
		DoCollisions();
	}
	else if (State == GAME_OVER)
	{
		SDL_Delay(2000);
		Score = 0; 
		State = GAME_ACTIVE;
	}
}

void Game::Render(SDL_Renderer *renderer)
{
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	if (State == GAME_ACTIVE)
	{
		snake->render(renderer);
		fruit->render(renderer);
		SDL_RenderPresent(renderer);
	}
	else if(State == GAME_OVER)
	{
		ScoreText = "Score: " + std::to_string(Score) + "   High score: " + std::to_string(HiScore);
		ScoreTextTexture.loadFromRenderedText(ScoreText, ResourceManager::GetFont("score_font"), { 255, 255, 255, 255 }, renderer);
		ScoreTextTexture.render((Width - ScoreTextTexture.getWidth()) / 2, (Height - ScoreTextTexture.getHeight()) / 2, renderer);
		SDL_RenderPresent(renderer);
		//SDL_Delay(2000);
		//Score = 0;
		//State = GAME_ACTIVE;
	}
	else
	{
		ScoreText = "Congratulations, you beat the game!";
		ScoreTextTexture.loadFromRenderedText(ScoreText, ResourceManager::GetFont("score_font"), { 255, 255, 255, 255 }, renderer);
		ScoreTextTexture.render((Width - ScoreTextTexture.getWidth()) / 2, (Height - ScoreTextTexture.getHeight()) / 2, renderer);
		SDL_RenderPresent(renderer);
		//SDL_Delay(4000);
		//Score = 0;
		//State = GAME_ACTIVE;
	}
}

void Game::DoCollisions()
{
	// Check if snake has collided with a wall or with its tail
	// wall:
	Segment head = snake->segments[0];
	if (head.x < 0 || head.x + head.size > Width || head.y < 0 || head.y + head.size > Height)
	{
		Mix_PlayChannel(-1, ResourceManager::GetChunk("game_over"), 0);
		State = GAME_OVER;
		ResetFruit();
		ResetSnake();
		return;
	}
	// tail:
	for (std::vector<Segment>::size_type i = 1; i < snake->segments.size(); ++i)
	{
		bool collidedWithTail = (head.x == snake->segments[i].x && head.y == snake->segments[i].y);
		
		if (collidedWithTail)
		{
			Mix_PlayChannel(-1, ResourceManager::GetChunk("game_over"), 0);
			State = GAME_OVER;
			ResetFruit();
			ResetSnake();
			return;
		}
	}
	
	// Check if snake collides with fruit
	if (State == GAME_ACTIVE)
	{
		if (head.x == fruit->x && head.y == fruit->y)
		{
			if (snake->segments.size() != NUMBER_OF_CELLS_X * NUMBER_OF_CELLS_Y - 1)
			{
				Mix_PlayChannel(-1, ResourceManager::GetChunk("eat_fruit"), 0);
				Segment newSegment{ CELL_SIZE, snake->segments.back().x, snake->segments.back().y, NONE };
				snake->segments.push_back(newSegment);
				ResetFruit();
				++Score;
				if (Score > HiScore)
					HiScore = Score;
			}
			else
			{
				Mix_PlayChannel(-1, ResourceManager::GetChunk("game_won"), 0);
				State = GAME_WON;
				++Score;
				if (Score > HiScore)
					HiScore = Score;
				ResetFruit();
				ResetSnake();
				return;
			}
		}
	}
}

void Game::ResetFruit()
{
	bool validPos;
	do
	{
		validPos = true;
		fruit->x = ux(e) * CELL_SIZE;
		fruit->y = uy(e) * CELL_SIZE;
		for (Segment &segment : snake->segments)
		{
			if (fruit->x == segment.x && fruit->y == segment.y)
			{
				validPos = false;
				break;
			}
		}
	} while (!validPos);
}

void Game::ResetSnake()
{
	snake->segments.clear();
	*snake = Snake();
}