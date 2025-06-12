#include "Tile.hpp"
#include "Engine/Resources.hpp"

static std::string GetTileImage(TileType type) {
    switch (type) {
        case TileType::Ground:
            return "play/floor.png";
        case TileType::Air:
            return "";
        default:
            return "";
    }
}

Tile::Tile(TileType t, int tileX, int tileY, int tileSize)
    : Engine::Sprite(GetTileImage(t), tileX * TILE_SIZE, tileY * TILE_SIZE, TILE_SIZE, TILE_SIZE),
      type(t) {
}

void Tile::Update(float deltaTime) {
    Engine::Sprite::Update(deltaTime);
    // Add per-tile behaviors here.
}