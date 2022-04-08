#pragma once

#include <math.h>
#include "globals.h"
#include "rectangle.h"

class Slope
{
public:
	Slope() {};
	Slope(Vector2 p1, Vector2 p2) :
		_p1(p1),
		_p2(p2)
	{
		if (this->_p2.x - this->_p1.x != 0) 
		{
			this->_slope = (fabs(this->_p2.y) - fabs(this->_p1.y)) / (fabs(this->_p2.x) - fabs(this->_p1.x));
		}
	}

	inline const float getSlope() const { return _slope; }

	const bool collidesWith(const Rectangle& other) const
	{ 
		return
			(other.getRight() >= this->_p2.x &&
				other.getLeft() <= this->_p1.x &&
				other.getTop() <= this->_p2.y &&
				other.getBottom() >= this->_p1.y) ||
			(other.getRight() >= this->_p1.x &&
				other.getLeft() <= this->_p2.x &&
				other.getTop() <= this->_p1.y &&
				other.getBottom() >= this->_p2.y) ||
			(other.getLeft() <= this->_p1.x &&
				other.getRight() >= this->_p2.x &&
				other.getTop() <= this->_p1.y &&
				other.getBottom() >= this->_p2.y) ||
			(other.getLeft() <= this->_p2.x &&
				other.getRight() >= this->_p1.x &&
				other.getTop() <= this->_p2.y &&
				other.getBottom() >= this->_p1.y);
	}

	inline const Vector2 getP1() const { return this->_p1; }
	inline const Vector2 getP2() const { return this->_p2; }

private:
	Vector2 _p1, _p2;
	float _slope;
};