#pragma once
#include "animatedSprite.h"
#include "slope.h"
#include "door.h"
#include <iostream>

class Graphics;
class Level;
class Enemy;

class Player : public AnimatedSprite
{
public:
	Player();
	Player(Graphics& graphics, Vector2 spawnPoint);
	void draw(Graphics& graphics);
	void update(float elapsedTime);

	/* void moveLeft
	*  Moves the player left by dx
	*/
	void moveLeft();

	/* void moveRight
	*  Moves the player right by dx
	*/
	void moveRight();

	/* void stopMoving
	*  Stops moving the player
	*/
	void stopMoving();

	void lookUp();
	void stopLookingUp();
	void lookDown();
	void stopLookingDown();

	void jump();

	virtual void animationDone(std::string currentAnimation) override;
	virtual void setupAnimations() override;

	void handleTileCollisions(std::vector<Rectangle>& others);
	void handleSlopeCollisions(std::vector<Slope>& others);
	void handleDoorCollision(std::vector<Door>& others, Level& level, Graphics& graphics);
	void handleEnemyCollisions(std::vector<Enemy*>& others);

	inline const float getX() const { return _x; };
	inline const float getY() const { return _y; };
	inline const int getMaxHealth() const { return _maxHealth; }
	inline const int getCurrentHealth() const { return _currentHealth; }

	void gainHealth(int amount);

private:
	float _dx, _dy;
	Direction _facing;
	bool _grounded;
	bool _lookingUp;
	bool _lookingDown;
	int _maxHealth;
	int _currentHealth;
};