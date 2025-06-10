#include "Tile.hpp"
#include "Engine/Resources.hpp"

static std::string GetTileImage(TileType type) {
    switch (type) {
        case TileType::Ground:
            return "play/floor.png";
        case TileType::Obstacle:
            return "play/dirt.png";
        case TileType::Background:
        default:
            return "play/Background.png";
    }
}

Tile::Tile(TileType t, int tileX, int tileY, int tileSize)
    : Engine::Sprite(GetTileImage(t), tileX * tileSize, tileY * tileSize, tileSize, tileSize),
      type(t) {
}

void Tile::Update(float deltaTime) {
    Engine::Sprite::Update(deltaTime);
    // Add per-tile behaviors here.
}