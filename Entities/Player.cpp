#include "Player.hpp"

#include <iostream>
#include <ostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <array>

#include "Game.hpp"
#include "Engine/Group.hpp"
#include "Engine/GameEngine.hpp"
#include "States/Gameplay.hpp"
#include "Utils/Config.hpp"
#include <cmath>
#include <iostream>

Player::Player(float x, float y)
    : Engine::Sprite("play/square.png", x, y), // 角色圖片請根據你資源路徑修改
      hp(3),
      velocityY(0.0f),
      gravity(1400.0f),    // 重力加速度（單位：像素/秒²）
      jumpSpeed(-700.0f), // 跳躍速度（負值代表向上）
      isOnGround(false) {
    upsideDown = false;
}

void Player::Update(float deltaTime) {
    if (!upsideDown) {
        velocityY += gravity * deltaTime;
        Position.y += velocityY * deltaTime;
    } else {
        velocityY -= gravity * deltaTime;
        Position.y += velocityY * deltaTime;
    }

    if (!isOnGround) {
        rotationAngle += 360 * deltaTime; // 每秒轉一圈，可調整轉速
        rotationAngle = fmod(rotationAngle, 360.0f); // 保持在 0~360°
    } else {
        // 若在地面，角度對齊（0 或 180，看你上下顛倒狀態）
        rotationAngle = upsideDown ? 180.0f : 0.0f;
    }

    isOnGround = false;

    Engine::Sprite::Update(deltaTime);
}

void Player::Land(float groundY) {
    Position.y = groundY - HitboxSize / 2.0f;
    velocityY = 0;
    isOnGround = true;
}

void Player::LandOnCeiling(float ceilingY) {
    Position.y = ceilingY + HitboxSize / 2.0f;
    velocityY = 0;
    isOnGround = true;
}

void Player::Jump() {
    if (isOnGround) {
        velocityY = jumpSpeed;
        isOnGround = false;
    }
}

void Player::Flip() {
    upsideDown = !upsideDown;
    jumpSpeed = -jumpSpeed;
}


int Player::GetHP() const {
    return hp;
}

void Player::ReduceHP(int value) {
    hp -= value;
    if (hp < 0) hp = 0;
}

void Player::SetHP(int value) {
    hp = value;
}

void Player::Draw() const {
    //Sprite::Draw();
    if (!bmp) return;

    float bmpW = al_get_bitmap_width(bmp.get());
    float bmpH = al_get_bitmap_height(bmp.get());

    float cx = bmpW * Anchor.x;
    float cy = bmpH * Anchor.y;

    al_draw_tinted_scaled_rotated_bitmap(bmp.get(),Tint,cx, cy,Position.x, Position.y,Size.x / bmpW, Size.y / bmpH,rotationAngle * ALLEGRO_PI / 180.0f, 0);

    if (showHitbox) {
        auto pts = GetHitboxPoints();
        for (size_t i = 0; i < pts.size(); ++i) {
            auto p1 = pts[i];
            auto p2 = pts[(i + 1) % pts.size()];
            al_draw_line(p1.x, p1.y, p2.x, p2.y, al_map_rgb(255, 0, 0), 2);
        }
        al_draw_circle(Position.x, Position.y, GroundHitboxRadius, al_map_rgb(0, 255, 0), 2);
    }
}

Engine::Point Player::GetHitboxTopLeft() const {
    auto pts = GetHitboxPoints();
    float minX = pts[0].x, minY = pts[0].y;
    for (auto& p : pts) {
        if (p.x < minX) minX = p.x;
        if (p.y < minY) minY = p.y;
    }
    return Engine::Point(minX, minY);
}

Engine::Point Player::GetHitboxBottomRight() const {
    auto pts = GetHitboxPoints();
    float maxX = pts[0].x, maxY = pts[0].y;
    for (auto& p : pts) {
        if (p.x > maxX) maxX = p.x;
        if (p.y > maxY) maxY = p.y;
    }
    return Engine::Point(maxX, maxY);
}

std::array<Engine::Point, 4> Player::GetHitboxPoints() const {
    float half = HitboxSize / 2.0f;
    float rad = rotationAngle * ALLEGRO_PI / 180.0f;
    float c = std::cos(rad);
    float s = std::sin(rad);
    std::array<Engine::Point, 4> pts{
        Engine::Point(Position.x - half, Position.y - half),
        Engine::Point(Position.x + half, Position.y - half),
        Engine::Point(Position.x + half, Position.y + half),
        Engine::Point(Position.x - half, Position.y + half)
    };
    std::array<Engine::Point, 4> local = {
        Engine::Point(-half, -half),
        Engine::Point( half, -half),
        Engine::Point( half,  half),
        Engine::Point(-half,  half)
    };
    for (size_t i = 0; i < 4; ++i) {
        float x = local[i].x * c - local[i].y * s;
        float y = local[i].x * s + local[i].y * c;
        pts[i] = Engine::Point(Position.x + x, Position.y + y);
    }
    return pts;
}

float Player::CalculateBottomY(const Engine::Point& pos, float angle) {
    float half = HitboxSize / 2.0f;
    float rad = angle * ALLEGRO_PI / 180.0f;
    float c = std::cos(rad);
    float s = std::sin(rad);
    float maxY = -1e9;
    std::array<Engine::Point, 4> local = {
        Engine::Point(-half, -half),
        Engine::Point( half, -half),
        Engine::Point( half,  half),
        Engine::Point(-half,  half)
    };
    for (auto& p : local) {
        float y = p.x * s + p.y * c;
        maxY = std::max(maxY, pos.y + y);
    }
    return maxY;
}

float Player::CalculateTopY(const Engine::Point& pos, float angle) {
    float half = HitboxSize / 2.0f;
    float rad = angle * ALLEGRO_PI / 180.0f;
    float c = std::cos(rad);
    float s = std::sin(rad);
    float minY = 1e9;
    std::array<Engine::Point, 4> local = {
        Engine::Point(-half, -half),
        Engine::Point( half, -half),
        Engine::Point( half,  half),
        Engine::Point(-half,  half)
    };
    for (auto& p : local) {
        float y = p.x * s + p.y * c;
        minY = std::min(minY, pos.y + y);
    }
    return minY;
}
