#pragma once
#include <string>
#include <vector>
#include "globals.h"
#include "tile.h"
#include "rectangle.h"
#include "slope.h"

class Graphics;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

//---------------------------------------------------------------------------------------------
/* Tile class is in level.h because a linker error will pop up everytime we try to call Tile in
* level.cpp. However, Tile class can be called in every other files except level.cpp. I can not
*  fix this error so I have to put Tile class in here.
*/
class Tile
{
public:
	Tile();
	Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);
	void update(float elapsedTime);
	void draw(Graphics& graphics);

private:
	SDL_Texture* _tileset;
	Vector2 _size;
	Vector2 _tilesetPosition;
	Vector2 _position;
};
//---------------------------------------------------------------------------------------------

class Level
{
public:
	Level();
	Level(std::string mapName, Vector2 spawnPoint, Graphics& graphics);
	~Level();

	void update(float elapsedTime);
	void draw(Graphics& graphics);

	std::vector<Rectangle> checkTileCollisions(const Rectangle& other);
	std::vector<Slope> checkSlopeCollisions(const Rectangle& other);
	inline const Vector2 getPlayerSpawnPoint() const { return this->_spawnPoint; }

private:
	std::string _mapName;
	Vector2 _spawnPoint;

	Vector2 _size;
	Vector2 _tileSize;

	SDL_Texture* _backgroundTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _collisionRects;
	std::vector<Slope> _slopes;

	/* void loadMap
	*  Loads a map;
	*/
	void loadMap(std::string mapName, Graphics& graphics);
};

// Tileset structure
struct Tileset
{
	SDL_Texture* Texture;
	int FirstGid;

	Tileset()
	{
		this->FirstGid = -1;
	}

	Tileset(SDL_Texture* texture, int firstGid)
	{
		this->Texture = texture;
		this->FirstGid = firstGid;
	}
};