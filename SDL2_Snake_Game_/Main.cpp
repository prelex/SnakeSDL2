#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <ctime>
#include "Snake.h"
#include "Fruit.h"

extern const int SCREEN_WIDTH = 600;
extern const int SCREEN_HEIGHT = 600;
extern const int CELL_SIZE = 40;
extern const int NUMBER_OF_CELLS_X = SCREEN_WIDTH / CELL_SIZE;
extern const int NUMBER_OF_CELLS_Y = SCREEN_HEIGHT / CELL_SIZE;

bool init();
void close();

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame = 0.0f;

float timer = 0.0f, delay = 0.07f;

Game SnakeGame(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* args[])
{
	if (!init())
		std::cout << "Failed to initialize" << std::endl;
	else
	{
		if (!SnakeGame.Init())
			std::cout << "Failed to initialise game" << std::endl;
		else
		{
			bool quit = false;

			SDL_Event e;

			// Game loop
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
						quit = true;

					SnakeGame.ProcessInput(e);
				}

				// Update screen 15 times per second
				currentFrame = SDL_GetTicks() / 1000.0f;
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;
				timer += deltaTime;
				if (timer > delay)
				{
					SnakeGame.Update();
					SnakeGame.Render(renderer);
					timer = 0.0f;
				}
			}
		}
	}

	close();

	return 0;
}

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL could not initialize. SDL error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == nullptr)
		{
			std::cout << "Main window could not be created. SDL error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			{
				printf("Warning: Linear texture filtering not enabled!");
			}

			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == nullptr)
			{
				std::cout << "Renderer could not be created. SDL error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					std::cout << "SDL_mixer could not initialize. SDL_mixer error: " << Mix_GetError() << std::endl;
					success = false;
				}

				if (TTF_Init() == -1)
				{
					std::cout << "SDL_ttf could not initialize. SDL_ttf error: " << TTF_GetError() << std::endl;
					success = false;
				}
			}

		}
	}

	return success;
}

void close()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	Mix_Quit();
	SDL_Quit();
	TTF_Quit();
}