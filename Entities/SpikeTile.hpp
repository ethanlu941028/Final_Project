#ifndef SPIKE_TILE_HPP
#define SPIKE_TILE_HPP

#include "Tile.hpp"

class SpikeTile : public Tile {
public:
    SpikeTile(int tileX, int tileY);
    Engine::Point GetBaseHitboxTopLeft() const;
    Engine::Point GetBaseHitboxBottomRight() const;
    Engine::Point GetTopHitboxTopLeft() const;
    Engine::Point GetTopHitboxBottomRight() const;
};

#endif // SPIKE_TILE_HPP