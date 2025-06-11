#include "AirTile.hpp"

AirTile::AirTile(int tileX, int tileY, int tileSize)
    : Tile(TileType::Air, tileX, tileY, tileSize) {}