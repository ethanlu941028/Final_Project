#include "SpikeTile.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "Utils/Config.hpp"

SpikeTile::SpikeTile(int tileX, int tileY, bool upsideDown)
    : Tile(TileType::Spike, tileX, tileY), upsideDown(upsideDown) {
    if (upsideDown) {
        Rotation = ALLEGRO_PI;
    }
}

void SpikeTile::Draw() const {
    Tile::Draw();
    if (showHitbox) {
        Engine::Point tl = GetBaseHitboxTopLeft();
        Engine::Point br = GetBaseHitboxBottomRight();
        al_draw_rectangle(tl.x, tl.y, br.x, br.y, al_map_rgb(0, 0, 255), 1);
        tl = GetTopHitboxTopLeft();
        br = GetTopHitboxBottomRight();
        al_draw_rectangle(tl.x, tl.y, br.x, br.y, al_map_rgb(0, 0, 255), 1);
    }
}

Engine::Point SpikeTile::GetBaseHitboxTopLeft() const {
    float left = Position.x - Size.x / 2.0f + 3;
    float top;
    if (upsideDown) {
        top = Position.y - Size.y / 2.0f;
    } else {
        top = Position.y + Size.y / 2.0f - 6;
    }
    return Engine::Point(left, top);
}

Engine::Point SpikeTile::GetBaseHitboxBottomRight() const {
    float right = Position.x + Size.x / 2.0f - 3;
    float bottom;
    if (upsideDown) {
        bottom = Position.y - Size.y / 2.0f + 6;
    } else {
        bottom = Position.y + Size.y / 2.0f;
    }
    return Engine::Point(right, bottom);
}

Engine::Point SpikeTile::GetTopHitboxTopLeft() const {
    float left = Position.x - 2;
    float top;
    if (upsideDown) {
        top = Position.y + Size.y / 2.0f - 4 - 30;
    } else {
        top = Position.y - Size.y / 2.0f + 4;
    }
    return Engine::Point(left, top);
}

Engine::Point SpikeTile::GetTopHitboxBottomRight() const {
    float right = Position.x + 2;
    float bottom;
    if (upsideDown) {
        bottom = Position.y + Size.y / 2.0f;
    } else {
        bottom = Position.y - Size.y / 2.0f + 4 + 30;
    }
    return Engine::Point(right, bottom);
}