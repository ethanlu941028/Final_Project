#ifndef TILE_HPP
#define TILE_HPP

#include "Engine/Sprite.hpp"
#include "Utils/Config.hpp"
#include <string>

enum class TileType {
    Air = 0,
    Ground = 1,
    Spike = 2,
    Flip = 3,
    Jump = 4,
    SpeedUp = 5,
    SpeedDown = 6
};

class Tile : public Engine::Sprite {
private:
    TileType type;
public:
    Tile(TileType type, int tileX, int tileY, int tileSize = TILE_SIZE);
    TileType GetType() const { return type; }
    void Update(float deltaTime) override;
    Engine::Point GetHitboxTopLeft() const;
    Engine::Point GetHitboxBottomRight() const;
    void Draw() const override;
};

#endif // TILE_HPP