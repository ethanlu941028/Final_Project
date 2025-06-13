#ifndef FLIPORB_HPP
#define FLIPORB_HPP

#include "Tile.hpp"
#include "Utils/Config.hpp"

class FlipOrb : public Tile {
public:
    static constexpr float HitboxRadius = 35.0f;
    FlipOrb(int tileX, int tileY);
    float GetRadius() const { return HitboxRadius; }
    void Draw() const override;
};

#endif // FLIPORB_HPP