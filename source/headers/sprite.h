#pragma once
#include <string>
#include <SDL.h>
#include "rectangle.h"
#include "globals.h"
/* Sprite class
*  Holds all info. for individual sprites
*/

class Graphics;

class Sprite
{
public:
	Sprite();
	Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY);

	virtual ~Sprite();
	virtual void update();
	void draw(Graphics& graphics, int x, int y);

	inline const Rectangle getBoundingBox() const { return _boundingBox; }
	const sides::Side getCollisionSide(Rectangle& other) const;

	inline const float getX() const { return _x; }
	inline const float getY() const { return _y; }

	void setSourceRectX(int value);
	void setSourceRectY(int value);

protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;
	Rectangle _boundingBox;
	float _x, _y;
};