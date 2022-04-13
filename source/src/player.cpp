#include "player.h"
#include "graphics.h"
#include "level.h"
#include "enemy.h"
#include <iostream>

namespace player_constants
{
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.7f;
	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}

Player::Player()
{
}

Player::Player(Graphics& graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "content/sprites/MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	_dx(0.f),
	_dy(0.f),
	_facing(RIGHT),
	_grounded(false),
	_maxHealth(3),
	_currentHealth(3)
{
	graphics.loadImage("content/sprites/MyChar.png");
	this->setupAnimations();
	this->playAnimation("RunRight");
}

void Player::draw(Graphics& graphics)
{
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Player::update(float elapsedTime)
{
	// Apply gravity
	if (this->_dy <= player_constants::GRAVITY_CAP)
	{
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}

	// Move by dx
	this->_x += this->_dx * elapsedTime;

	// Move by dy
	this->_y += this->_dy * elapsedTime;

	AnimatedSprite::update(elapsedTime);
}

void Player::moveLeft()
{
	if(this->_lookingDown && this->_grounded)
	{
		return;
	}
	this->_dx = -player_constants::WALK_SPEED;
	if (!this->_lookingUp)
	{
		this->playAnimation("RunLeft");
	}
	this->_facing = LEFT;
}

void Player::moveRight()
{
	if (this->_lookingDown && this->_grounded)
	{
		return;
	}
	this->_dx = player_constants::WALK_SPEED;
	if (!this->_lookingUp)
	{
		this->playAnimation("RunRight");
	}
	this->_facing = RIGHT;
}

void Player::stopMoving()
{
	this->_dx = 0.0f;
	if (!this->_lookingUp && !this->_lookingDown)
	{
		if (this->_facing == LEFT)
		{
			this->playAnimation("IdleLeft");
		}
		else
		{
			this->playAnimation("IdleRight");
		}
	}
}

void Player::lookUp()
{
	this->_lookingUp = true;
	if (this->_dx == 0)
	{
		this->playAnimation(this->_facing == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else
	{
		this->playAnimation(this->_facing == RIGHT ? "RunRightUp" : "RunLeftUp");
	}
}

void Player::stopLookingUp()
{
	this->_lookingUp = false;
}

void Player::lookDown()
{
	this->_lookingDown = true;
	if (this->_grounded)
	{
		// We need to interact (turn backwards)
		this->playAnimation(this->_facing == RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
	}
	else
	{
		this->playAnimation(this->_facing == RIGHT ? "LookDownRight" : "LookDownLeft");
	}
}

void Player::stopLookingDown()
{
	this->_lookingDown = false;
}

void Player::jump()
{
	if (this->_grounded)
	{
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}
}

void Player::animationDone(std::string currentAnimation)
{

}

void Player::setupAnimations()
{
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 0, "IdleLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));
}

void Player::handleTileCollisions(std::vector<Rectangle>& others)
{
	// Figure out what side the collision happened on and move the player accordingly
	for (int i = 0; i < others.size(); i++)
	{
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE)
		{
			switch (collisionSide)
			{
			case sides::TOP:
				this->_dy = 0.f;
				this->_y = others.at(i).getBottom() + 1;
				if (this->_grounded)
				{
					this->_dx = 0;
					this->_x -= this->_facing == RIGHT ? 1.0f : -1.0f;
				}
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0.f;
				this->_grounded = true;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				break;
			case sides::NONE:
				break;
			default:
				break;
			}
		}
	}
}

void Player::handleSlopeCollisions(std::vector<Slope>& others)
{
	for (int i = 0; i < others.size(); i++)
	{
		// Calculate where on the slope the player's bottom center is touching
		int b = (others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x)));

		int centerX = this->_boundingBox.getCenterX();

		int newY = (others.at(i).getSlope() * centerX) + b - 8;

		if (this->_grounded)
		{
			this->_y = newY - this->_boundingBox.getHeight();
			this->_grounded = true;
		}
	}
}

void Player::handleDoorCollision(std::vector<Door>& others, Level& level, Graphics& graphics)
{
	for (int i = 0; i < others.size(); i++)
	{
		if (this->_grounded && this->_lookingDown)
		{
			level = Level(others.at(i).getDestination(), graphics);
			this->_x = level.getPlayerSpawnPoint().x;
			this->_y = level.getPlayerSpawnPoint().y;
		}
	}
}

void Player::handleEnemyCollisions(std::vector<Enemy*>& others)
{
	for (int i = 0; i < others.size(); i++)
	{
		others.at(i)->touchPlayer(*this);
	}
}

void Player::gainHealth(int amount)
{
	this->_currentHealth += amount;
}


