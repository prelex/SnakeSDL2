#pragma once
#include <SDL.h>
#include <sstream>
#include "Texture.h"

enum GameState {GAME_ACTIVE, GAME_OVER, GAME_WON};

class Game
{
public:
	int Width, Height;
	int Score, HiScore;
	GameState State;

	std::string ScoreText;
	Texture ScoreTextTexture;

	Game(int width, int height);

	bool Init();

	void ProcessInput(SDL_Event &e);
	void Update();
	void Render(SDL_Renderer *renderer);

	void DoCollisions();
	void ResetSnake();
	void ResetFruit();
};