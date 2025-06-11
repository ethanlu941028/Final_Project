#ifndef TILE_HPP
#define TILE_HPP

#include "Engine/Sprite.hpp"
#include <string>

enum class TileType {
    Air = 0,
    Ground = 1
};

class Tile : public Engine::Sprite {
private:
    TileType type;
public:
    Tile(TileType type, int tileX, int tileY, int tileSize = 64);
    TileType GetType() const { return type; }
    void Update(float deltaTime) override;
};

#endif // TILE_HPP