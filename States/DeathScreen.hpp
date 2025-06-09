#ifndef DEATH_SCREEN_HPP
#define DEATH_SCREEN_HPP

#include "Engine/IScene.hpp"

namespace Engine {
    class Label;
    class ImageButton;
}

class DeathScreen : public Engine::IScene {
private:
    Engine::Label* gameOverLabel;
    Engine::ImageButton* backToMenuButton;

    void BackToMenuOnClick();
public:
    explicit DeathScreen() = default;
    void Initialize() override;
    void Terminate() override;
};

#endif // DEATH_SCREEN_HPP
