#include "JumpOrb.hpp"
#include <allegro5/allegro_primitives.h>

JumpOrb::JumpOrb(int tileX, int tileY)
    : Tile(TileType::Jump, tileX, tileY) {
    CollisionRadius = HitboxRadius;
}

void JumpOrb::Draw() const {
    Tile::Draw();
    if (showHitbox) {
        al_draw_circle(Position.x, Position.y, HitboxRadius, al_map_rgb(0, 255, 0), 2);
    }
}