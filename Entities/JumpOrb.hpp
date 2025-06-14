#ifndef JUMPORB_HPP
#define JUMPORB_HPP

#include "Tile.hpp"
#include "Utils/Config.hpp"

class JumpOrb : public Tile {
public:
    static constexpr float HitboxRadius = 40.0f;
    JumpOrb(int tileX, int tileY);
    float GetRadius() const { return HitboxRadius; }
    void Draw() const override;
};

#endif // FLIPORB_HPP