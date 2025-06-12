#include "Player.hpp"

#include <iostream>
#include <ostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Game.hpp"
#include "Engine/Group.hpp"
#include "Engine/GameEngine.hpp"
#include "States/Gameplay.hpp"
#include <iostream>

Player::Player(float x, float y)
    : Engine::Sprite("play/square.png", x, y), // 角色圖片請根據你資源路徑修改
      hp(3),
      velocityY(0.0f),
      gravity(1800.0f),    // 重力加速度（單位：像素/秒²）
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

    Engine::Point tl = GetHitboxTopLeft();
    Engine::Point br = GetHitboxBottomRight();
    al_draw_rectangle(tl.x, tl.y, br.x, br.y, al_map_rgb(255, 0, 0), 2);
}

Engine::Point Player::GetHitboxTopLeft() const {
    return Engine::Point(Position.x - HitboxSize / 2.0f, Position.y - HitboxSize / 2.0f);
}

Engine::Point Player::GetHitboxBottomRight() const {
    return Engine::Point(Position.x + HitboxSize / 2.0f, Position.y + HitboxSize / 2.0f);
}
