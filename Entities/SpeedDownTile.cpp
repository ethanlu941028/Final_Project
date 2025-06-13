#include "SpeedDownTile.hpp"

SpeedDownTile::SpeedDownTile(int tileX, int tileY)
    : Tile(TileType::SpeedDown, tileX, tileY, TILE_SIZE) {
    Size = Engine::Point(TILE_SIZE, TILE_SIZE * 2);
    Position.y += TILE_SIZE / 2.0f;
}