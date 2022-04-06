#include <SDL.h>
#include <iostream>
#include "..\headers\graphics.h"


/* Graphics clss
*  Holds all info. dealing with graphics for the game
*/
Graphics::Graphics()
{
	SDL_CreateWindowAndRenderer(640, 480, 0, &this->_window, &this->_renderer);
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
