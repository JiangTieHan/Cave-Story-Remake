#pragma once

class SDL_Window;
class SDL_Renderer;

class Graphics
{
public:
	Graphics();
	~Graphics();
private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
};