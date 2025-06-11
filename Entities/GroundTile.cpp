#include "GroundTile.hpp"

GroundTile::GroundTile(int tileX, int tileY, int tileSize)
    : Tile(TileType::Ground, tileX, tileY, tileSize) {}