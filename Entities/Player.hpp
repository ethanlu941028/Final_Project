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
    explicit Player(float x = 100, float y = 400); // 預設位置
    void Update(float deltaTime) override;
    void Jump();
    int GetHP() const;
    void ReduceHP(int value);
    void SetHP(int value);
    void Draw() const override;
    bool upsideDown;
    void Flip();
};

#endif // PLAYER_HPP
