#include "Player.hpp"

#include <iostream>
#include <ostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Game.hpp"
#include "Engine/Group.hpp"
#include "Engine/GameEngine.hpp"

Player::Player(float x, float y)
    : Engine::Sprite("play/enemy-2.png", x, y), // 角色圖片請根據你資源路徑修改
      hp(3),
      velocityY(0.0f),
      gravity(800.0f),    // 重力加速度（單位：像素/秒²）
      jumpSpeed(-350.0f), // 跳躍速度（負值代表向上）
      isOnGround(false) {
}

void Player::Update(float deltaTime) {

    velocityY += gravity * deltaTime;
    Position.y += velocityY * deltaTime;

    // 假設地面為 y = 600
    if (Position.y >= 670) {
        Position.y = 670;
        velocityY = 0;
        isOnGround = true;
    }
    else if (Position.y <= 0) Position.y = 0;
    else {
        isOnGround = false;
    }

    Engine::Sprite::Update(deltaTime);
}
void Player::Jump() {
    velocityY = jumpSpeed;
    isOnGround = false;
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
    Sprite::Draw();
    al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
}
