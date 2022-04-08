#pragma once
#include "animatedSprite.h"
#include "slope.h"

class Graphics;

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

	void jump();

	virtual void animationDone(std::string currentAnimation) override;
	virtual void setupAnimations() override;

	void handleTileCollisions(std::vector<Rectangle>& others);
	void handleSlopeCollisions(std::vector<Slope>& others);

	inline const float getX() const { return _x; };
	inline const float getY() const { return _y; };

private:
	float _dx, _dy;
	Direction _facing;
	bool _grounded;
};