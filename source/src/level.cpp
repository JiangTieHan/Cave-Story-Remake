#include "level.h"
#include "graphics.h"
#include <SDL.h>

#include <tinyxml2.h>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace tinyxml2;

//---------------------------------------------------------------------------------------------
/* Tile class is in level.h because a linker error will pop up everytime we try to call Tile in
* level.cpp. However, Tile class can be called in every other files except level.cpp. I can not
*  fix this error so I have to put Tile class in here.
*/

Tile::Tile()
{
}

Tile::Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position) :
	_tileset(tileset),
	_size(size),
	_tilesetPosition(tilesetPosition),
	_position(Vector2(position.x* globals::SPRITE_SCALE, position.y* globals::SPRITE_SCALE))
{}

void Tile::update(float elapsedTime)
{
}

void Tile::draw(Graphics& graphics)
{
	SDL_Rect destRect = { this->_position.x, this->_position.y,
	this->_size.x * globals::SPRITE_SCALE, this->_size.y * globals::SPRITE_SCALE };

	SDL_Rect sourceRect = { this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y };

	graphics.blitSurface(this->_tileset, &sourceRect, &destRect);
}

//---------------------------------------------------------------------------------------------------------

Level::Level()
{
}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics& graphics) :
	_mapName(mapName),
	_spawnPoint(spawnPoint),
	_size(Vector2(0, 0))
{
	this->loadMap(mapName, graphics);
}

Level::~Level()
{
}

void Level::update(float elapsedTime)
{
}

void Level::draw(Graphics& graphics)
{
	for (int i = 0; i < this->_tileList.size(); i++)
	{
		this->_tileList.at(i).draw(graphics);
	}
}

void Level::loadMap(std::string mapName, Graphics& graphics)
{
	// Pare the tmx file
	XMLDocument doc;
	std::stringstream ss;
	ss << "content/maps/" << mapName << ".tmx";
	doc.LoadFile(ss.str().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");

	// Get the width and height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);

	// Get the width and the height of the tiles and store it in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	// Load the tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset)
	{
		while (pTileset)
		{
			int firstgid;
			const char* source = pTileset->FirstChildElement("image")->Attribute("source");
			char* path;
			std::stringstream ss;
			ss << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			if (!tex)
			{
				std::cout << "Level::loadMap: load " << source << " into tex failed!" << std::endl;
				return;
			}
			this->_tilesets.push_back(Tileset(tex, firstgid));

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	// Loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer)
	{
		while (pLayer)
		{
			// loading the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData)
			{
				while (pData)
				{
					// loading the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile)
					{
						int tileCounter = 0;
						while (pTile)
						{
							// Build each individual tile here
							// if gid is 0, no tile should be drawn. Continue loop
							if (pTile->IntAttribute("gid") == 0)
							{
								tileCounter++;
								if (pTile->NextSiblingElement("tile"))
								{
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else
								{
									break;
								}
							}

							// Get the tileset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							for (int i = 0; i < this->_tilesets.size(); i++)
							{
								if (this->_tilesets[i].FirstGid <= gid)
								{
									tls = this->_tilesets.at(i);
									break;
								}
							}
							if (tls.FirstGid == -1)
							{
								// No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile"))
								{
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else
								{
									break;
								}

							}

							// Get the position of the tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							Vector2 finalTilePosition = Vector2(xx, yy);

							// Calculate the position of the tile in the tileset
							int tilesetWidth, tilesetHeight;
							SDL_QueryTexture(tls.Texture, nullptr, nullptr, &tilesetWidth, &tilesetHeight);
							int tsxx = gid % (tilesetWidth / tileWidth) - 1;
							tsxx *= tileWidth;

							int tsyy = (gid / (tilesetWidth / tileWidth)) * tileHeight;
							Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);



							// Build the actual tile and add it to level's tile list
							Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
							this->_tileList.push_back(tile);
							tileCounter++;

							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}
}
