#include <iostream>
#include "sprite.h"
#include "graphics.h"
#include "globals.h"
#pragma once

Sprite::Sprite(){}

Sprite::Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height, 
	float posX, float posY) :
	_x(posX),
	_y(posY)
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = width;
	this->_sourceRect.h = height;

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (!this->_spriteSheet)
	{
		std::cout << "Sprite::Sprite: Unable to load " << filePath << std::endl;
	}

	this->_boundingBox = Rectangle(this->_x, this->_y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
}

Sprite::~Sprite()
{
}

void Sprite::update()
{
	this->_boundingBox = Rectangle(this->_x, this->_y,
		this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE);
}

void Sprite::draw(Graphics& graphics, int x, int y)
{
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::SPRITE_SCALE,
		this->_sourceRect.h * globals::SPRITE_SCALE};
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

const sides::Side Sprite::getCollisionSide(Rectangle& other) const
{
	int amtRight, amtLeft, amtTop, amtBottom;
	amtRight = this->_boundingBox.getRight() - other.getLeft();
	amtLeft = other.getRight() - this->_boundingBox.getLeft();
	amtTop = other.getBottom() - this->_boundingBox.getTop();
	amtBottom = this->_boundingBox.getBottom() - other.getTop();

	int vals[4]{ abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
	int lowest = vals[0];
	for (int i = 0; i < 4; i++)
	{
		if (vals[i] < lowest)
		{
			lowest = vals[i];
		}
	}

	return
		lowest == abs(amtRight) ? sides::RIGHT :
		lowest == abs(amtLeft) ? sides::LEFT :
		lowest == abs(amtTop) ? sides::TOP :
		lowest == abs(amtBottom) ? sides::BOTTOM :
		sides::NONE;
}

void Sprite::setSourceRectX(int value)
{
	this->_sourceRect.x = value;
}

void Sprite::setSourceRectY(int value)
{
	this->_sourceRect.y = value;
}
