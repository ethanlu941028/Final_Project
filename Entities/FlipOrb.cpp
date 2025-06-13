#include "FlipOrb.hpp"
#include <allegro5/allegro_primitives.h>

FlipOrb::FlipOrb(int tileX, int tileY)
    : Tile(TileType::Flip, tileX, tileY) {
    CollisionRadius = HitboxRadius;
}

void FlipOrb::Draw() const {
    Tile::Draw();
    if (showHitbox) {
        al_draw_circle(Position.x, Position.y, HitboxRadius, al_map_rgb(0, 255, 0), 2);
    }
}