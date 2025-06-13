#include "SpeedUpTile.hpp"

SpeedUpTile::SpeedUpTile(int tileX, int tileY)
    : Tile(TileType::SpeedUp, tileX, tileY, TILE_SIZE) {
    Size = Engine::Point(TILE_SIZE, TILE_SIZE * 2);
    Position.y += TILE_SIZE / 2.0f;
}