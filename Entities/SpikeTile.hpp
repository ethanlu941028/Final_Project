#ifndef SPIKE_TILE_HPP
#define SPIKE_TILE_HPP

#include "Tile.hpp"

class SpikeTile : public Tile {
private:
    bool upsideDown;
public:
    explicit SpikeTile(int tileX, int tileY, bool upsideDown = false);
    bool IsUpsideDown() const { return upsideDown; }
    Engine::Point GetBaseHitboxTopLeft() const;
    Engine::Point GetBaseHitboxBottomRight() const;
    Engine::Point GetTopHitboxTopLeft() const;
    Engine::Point GetTopHitboxBottomRight() const;
    void Draw() const override;
};

#endif // SPIKE_TILE_HPP