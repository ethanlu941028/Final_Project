#include "Tile.hpp"
#include "Engine/Resources.hpp"
#include <allegro5/allegro_primitives.h>
#include "Utils/Config.hpp"

static std::string GetTileImage(TileType type) {
    switch (type) {
        case TileType::Ground:
            return "play/groundTile.png";
        case TileType::Air:
            return "";
        case TileType::Spike:
            return "play/spike.png";
        case TileType::Flip:
            return "play/FlipOrb.png";
        case TileType::Jump:
            return "play/JumpOrb.png";
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

Engine::Point Tile::GetHitboxTopLeft() const {
    return Engine::Point(Position.x - Size.x / 2.0f, Position.y - Size.y / 2.0f);
}

Engine::Point Tile::GetHitboxBottomRight() const {
    return Engine::Point(Position.x + Size.x / 2.0f, Position.y + Size.y / 2.0f);
}

void Tile::Draw() const {
    Sprite::Draw();
    if (showHitbox) {
        Engine::Point tl = GetHitboxTopLeft();
        Engine::Point br = GetHitboxBottomRight();
        al_draw_rectangle(tl.x, tl.y, br.x, br.y, al_map_rgb(0, 255, 0), 1);
    }
}