#ifndef DEATH_SCREEN_HPP
#define DEATH_SCREEN_HPP

#include "Engine/IScene.hpp"

namespace Engine {
    class Label;
    class ImageButton;
    class Image;
}

class DeathScreen : public Engine::IScene {
private:
    Engine::Label* gameOverLabel;
    Engine::ImageButton* backToMenuButton;
    Engine::Image* background;

    void BackToMenuOnClick();
public:
    explicit DeathScreen() = default;
    void Initialize() override;
    void Terminate() override;
    void OnKeyDown(int key) override;
};

#endif // DEATH_SCREEN_HPP
