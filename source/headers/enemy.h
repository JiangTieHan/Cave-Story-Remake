#pragma once

#include "animatedSprite.h"
#include "globals.h"
#include "player.h"

class Graphics;

class Enemy : public AnimatedSprite
{
public:
	Enemy();
	Enemy(Graphics& graphics, std::string filePath, int sourceX, int sourceY,
		int width, int height, Vector2 spawnPoint, int timeToUpdate);
	virtual void update(int elapsedTime, Player& player);
	virtual void draw(Graphics& graphics);
	virtual void touchPlayer(Player& player) = 0;

	inline const int getMaxHealth() const { return this->_maxHealth; }
	inline const int getCurrentHealth() const { return this->_currentHealth; }

protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;
};

class Bat : public Enemy
{
public:
	Bat();
	Bat(Graphics& graphics, Vector2 spawnPoint);
	void update(int elapsedTime, Player& player);
	void draw(Graphics& graphics);

	void animationDone(std::string currentAnimation);
	void setupAnimations();
	void touchPlayer(Player& player);

private:
	float _startingX, _startingY;
	bool _shouldMoveUp;
};