#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "graphics.h"
#include "globals.h"


/* Graphics clss
*  Holds all info. dealing with graphics for the game
*/
Graphics::Graphics()
{
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "Cavestroy");

	if (!this->_window)
	{
		std::cout << "Graphics::Graphics(): Failed to create window" << std::endl;
	}

	if (!this->_renderer)
	{
		std::cout << "Graphics::Graphics(): Failed to create renderer" << std::endl;
	}
}

Graphics::~Graphics()
{
	SDL_DestroyWindow(this->_window);
}

SDL_Surface* Graphics::loadImage(const std::string& filePath)
{
	if (this->_spriteSheets.count(filePath) == 0)
	{
		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	return this->_spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle)
{
	SDL_RenderCopy(this->_renderer, source, sourceRectangle, destinationRectangle);
}

void Graphics::flip()
{
	SDL_RenderPresent(this->_renderer);
}

void Graphics::clear()
{
	SDL_RenderClear(this->_renderer);
}
