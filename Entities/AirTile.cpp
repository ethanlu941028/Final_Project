#include "AirTile.hpp"

AirTile::AirTile(int tileX, int tileY) : Tile(TileType::Air, tileX, tileY) {}

void AirTile::Draw() const {
    // Air tiles are invisible and have no hitbox.
}