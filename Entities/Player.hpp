#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Engine/Sprite.hpp"
#include <array>

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
    static constexpr float GroundHitboxRadius = HitboxSize * 0.4f;
    explicit Player(float x = 100, float y = 400); // 預設位置
    void Update(float deltaTime) override;
    void Jump();
    void OrbJump();
    int GetHP() const;
    void ReduceHP(int value);
    void SetHP(int value);
    float GetVelocityY() const { return velocityY; }
    void SetVelocityY(float v) { velocityY = v; }
    void Land(float groundY);
    void LandOnCeiling(float ceilingY);
    Engine::Point GetHitboxTopLeft() const;
    Engine::Point GetHitboxBottomRight() const;
    std::array<Engine::Point, 4> GetHitboxPoints() const;
    static float CalculateBottomY(const Engine::Point& pos, float angle);
    static float CalculateTopY(const Engine::Point& pos, float angle);
    float GetBottomY() const { return CalculateBottomY(Position, rotationAngle); }
    float GetTopY() const { return CalculateTopY(Position, rotationAngle); }
    float GetGroundRadius() const { return GroundHitboxRadius; }
    void Draw() const override;
    bool upsideDown;
    void Flip();
    float rotationAngle = 0.0f;
};

#endif // PLAYER_HPP
