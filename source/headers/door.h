#pragma once

#include "rectangle.h"
#include "globals.h"
#include <string>

class Door : public Rectangle
{
public:
	Door() {}
	Door(Rectangle r, std::string destination) :
		Rectangle(r.getLeft() * globals::SPRITE_SCALE, r.getTop() * globals::SPRITE_SCALE,
			r.getWidth() * globals::SPRITE_SCALE, r.getHeight() * globals::SPRITE_SCALE),
		_destination(destination) {}

	inline const Rectangle getRectangle() const { return this->getRect(); }
	inline const std::string getDestination() const { return this->_destination; }

private:
	std::string _destination;
};