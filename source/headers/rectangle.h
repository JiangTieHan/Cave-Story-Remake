#pragma once

#include "globals.h"

class Rectangle
{
public:
	Rectangle(){}
	Rectangle(float x, float y, float width, float height) :
		_x(x),
		_y(y),
		_width(width),
		_height(height){}

	inline const float getCenterX() const { return this->_x + this->_width / 2; }
	inline const float getCenterY() const { return this->_y + this->_height / 2; }

	inline const float getLeft() const { return this->_x; }
	inline const float getRight() const { return this->_x + this->_width; }
	inline const float getTop() const { return this->_y; }
	inline const float getBottom() const { return this->_y + this->_height; }

	inline const float getWidth() const { return _width; }
	inline const float getHeight() const { return _height; }

	inline const float getSide(const sides::Side side) const
	{
		return
			side == sides::LEFT ? this->getLeft() :
			side == sides::RIGHT ? this->getRight() :
			side == sides::TOP ? this->getTop() :
			side == sides::BOTTOM ? this->getBottom() :
			sides::NONE;
	}

	/* bool collidesWith
	*  Takes in another Rectangle and checks if the two are colliding
	*/
	inline const bool collidesWith(const Rectangle& other) const
	{
		return
			this->getRight() >= other.getLeft() &&
			this->getLeft() <= other.getRight() &&
			this->getTop() <= other.getBottom() &&
			this->getBottom() >= other.getTop();
	}

	inline const bool isValidRectangle() const
	{
		return (this->_x >= 0 && this->_y >= 0 && this->_width >= 0 && this->_height >= 0);
	}

	inline const Rectangle getRect() const { return *this; }

private:
	int _x, _y, _width, _height;
};