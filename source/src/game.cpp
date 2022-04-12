#include <SDL.h>
#include "game.h"
#include "graphics.h"
#include "input.h"
#include "iostream"

/* Game class
*  This class holds all info. for our main game loop
*/

namespace
{
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
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

	this->_level = Level("Map 1", graphics);
	this->_hud = HUD(graphics, _player);
	this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());

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

		else if (input.isKeyHeld(SDL_SCANCODE_A))
		{
			this->_player.moveLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_D))
		{
			this->_player.moveRight();
		}
		if (input.isKeyHeld(SDL_SCANCODE_W))
		{
			this->_player.lookUp();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_S))
		{
			this->_player.lookDown();
		}

		if (input.wasKeyReleased(SDL_SCANCODE_W))
		{
			this->_player.stopLookingUp();
		}
		if (input.wasKeyReleased(SDL_SCANCODE_S))
		{
			this->_player.stopLookingDown();
		}
		if (input.wasKeyPressed(SDL_SCANCODE_SPACE))
		{
			this->_player.jump();
		}
		if (!input.isKeyHeld(SDL_SCANCODE_A) && !input.isKeyHeld(SDL_SCANCODE_D))
		{
			this->_player.stopMoving();
		}

		// Calculate elapsed time
		const int CRRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CRRENT_TIME_MS - LAST_UPDATE_TIME;
		this->_graphics = &graphics;
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CRRENT_TIME_MS;

		this->draw(graphics);
	}
}

void Game::draw(Graphics& graphics)
{
	graphics.clear();

	this->_level.draw(graphics);
	this->_player.draw(graphics);

	this->_hud.draw(graphics);

	graphics.flip();
}

void Game::update(float elapsedTime)
{
	this->_level.update(elapsedTime);
	this->_player.update(elapsedTime);
	this->_hud.update(elapsedTime);

	// Check collisions
	std::vector<Rectangle> others;
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0)
	{
		// Player collided with at least one tile. Handle it.
		this->_player.handleTileCollisions(others);
	}

	std::vector<Slope> otherSlopes;
	if ((otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0)
	{
		this->_player.handleSlopeCollisions(otherSlopes);
	}

	std::vector<Door> otherDoors;
	if ((otherDoors = this->_level.checkDoorCollisions(this->_player.getBoundingBox())).size() > 0)
	{
		this->_player.handleDoorCollision(otherDoors, this->_level, *this->_graphics);
	}
}
