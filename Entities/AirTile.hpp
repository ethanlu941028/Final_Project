#ifndef AIR_TILE_HPP
#define AIR_TILE_HPP

#include "Tile.hpp"

class AirTile : public Tile {
public:
    AirTile(int tileX, int tileY);
    void Draw() const override;
};

#endif // AIR_TILE_HPP