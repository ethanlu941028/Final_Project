#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Engine/Sprite.hpp"

class Player : public Engine::Sprite {
private:
    int hp;
    float velocityY;
    float gravity;
    float jumpSpeed;
    bool isOnGround;
    static bool Debugmode;

public:
    static constexpr int HitboxSize = 64;
    explicit Player(float x = 100, float y = 400); // 預設位置
    void Update(float deltaTime) override;
    void Jump();
    int GetHP() const;
    void ReduceHP(int value);
    void SetHP(int value);
    float GetVelocityY() const { return velocityY; }
    void SetVelocityY(float v) { velocityY = v; }
    void Land(float groundY);
    Engine::Point GetHitboxTopLeft() const;
    Engine::Point GetHitboxBottomRight() const;
    void Draw() const override;
    bool upsideDown;
    void Flip();
};

#endif // PLAYER_HPP
