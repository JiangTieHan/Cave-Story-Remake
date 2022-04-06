#include <SDL.h>
#include "game.h"
#include "graphics.h"
#include "input.h"

/* Game class
*  This class holds all info. for our main game loop
*/

namespace
{
	const int FPS = 50;
	const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	this->gameLoop();
}

Game::~Game()
{
}

void Game::gameLoop()
{
	Graphics graphics;
	Input input;
	SDL_Event event;

	this->_player = Sprite(graphics, "content/sprites/MyChar.png", 0, 0, 16, 16, 100, 100);

	bool running = true;
	int LAST_UPDATE_TIME = SDL_GetTicks();

	// Main game loop starts
	while (running)
	{
		input.beginNewFrame();

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.repeat == 0)
				{
					input.keyDownEvent(event);
				}
			}
			else
			{
				if (event.type == SDL_KEYUP)
				{
					input.keyUpEvent(event);
				}
			}

			if (event.type == SDL_QUIT)
			{
				running = false;
			}
		}

		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
		{
			running = false;
		}

		// Calculate elapsed time
		const int CRRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CRRENT_TIME_MS - LAST_UPDATE_TIME;
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CRRENT_TIME_MS;

		this->draw(graphics);
	}
}

void Game::draw(Graphics& graphics)
{
	graphics.clear();

	this->_player.draw(graphics, 100, 100);

	graphics.flip();
}

void Game::update(float elapsedTime)
{
}
