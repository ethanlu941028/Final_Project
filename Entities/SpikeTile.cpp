#include "SpikeTile.hpp"

SpikeTile::SpikeTile(int tileX, int tileY)
    : Tile(TileType::Spike, tileX, tileY) {}

Engine::Point SpikeTile::GetBaseHitboxTopLeft() const {
    float left = Position.x - Size.x / 2.0f + 3;
    float top = Position.y + Size.y / 2.0f - 6;
    return Engine::Point(left, top);
}

Engine::Point SpikeTile::GetBaseHitboxBottomRight() const {
    float right = Position.x + Size.x / 2.0f - 3;
    float bottom = Position.y + Size.y / 2.0f;
    return Engine::Point(right, bottom);
}

Engine::Point SpikeTile::GetTopHitboxTopLeft() const {
    float left = Position.x - 2;
    float top = Position.y - Size.y / 2.0f + 4;
    return Engine::Point(left, top);
}

Engine::Point SpikeTile::GetTopHitboxBottomRight() const {
    float right = Position.x + 2;
    float bottom = Position.y - Size.y / 2.0f + 4 + 30;
    return Engine::Point(right, bottom);
}