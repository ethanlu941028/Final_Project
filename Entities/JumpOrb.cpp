#include "JumpOrb.hpp"
#include <allegro5/allegro_primitives.h>

JumpOrb::JumpOrb(int tileX, int tileY)
    : Tile(TileType::Jump, tileX, tileY) {
    CollisionRadius = HitboxRadius;
}

void JumpOrb::Draw() const {
    // Draw a soft yellow glow behind the orb.
    ALLEGRO_COLOR glow = al_map_rgba(255, 220, 0, 180);
    al_draw_filled_circle(Position.x, Position.y, HitboxRadius, glow);
    Tile::Draw();
    if (showHitbox) {
        al_draw_circle(Position.x, Position.y, HitboxRadius, al_map_rgb(0, 255, 0), 2);
    }
}