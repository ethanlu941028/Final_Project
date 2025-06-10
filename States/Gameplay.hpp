#ifndef PLAY_SCENE_GAMEPLAY_HPP
#define PLAY_SCENE_GAMEPLAY_HPP

#include <allegro5/allegro.h>
#include "Engine/IScene.hpp"

namespace Engine {
    class Image;
    class Label;
    class ImageButton;
}

class Player;  // 前向宣告

class Gameplay : public Engine::IScene {
private:
    Engine::Image* background;
    Engine::Label* scoreLabel;
    Engine::ImageButton* pauseButton;

    int score;
    Player* player;


    void PauseOnClick();
    void CheckPlayerHealth();

public:
    bool initialized = false;
    explicit Gameplay() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void OnKeyDown(int keyCode) override;
    //void Draw() const override;
    //void OnMouseDown(int button, int mx, int my) override;
    //void OnEvent(ALLEGRO_EVENT& event) override;
};

#endif // PLAY_SCENE_GAMEPLAY_HPP
