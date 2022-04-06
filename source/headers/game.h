#pragma once
#include "animatedSprite.h"
class Graphics;

class Game
{
public:
	Game();
	~Game();

private:
	void gameLoop();
	void draw(Graphics& graphics);
	void update(float elapsedTime);

	AnimatedSprite _player;
};