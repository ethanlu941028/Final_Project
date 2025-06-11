#ifndef WIN_SCREEN_HPP
#define WIN_SCREEN_HPP

#include "Engine/IScene.hpp"

namespace Engine {
    class Label;
    class ImageButton;
    class Image;
}

class WinScreen : public Engine::IScene {
private:
    Engine::Label* gameOverLabel;
    Engine::ImageButton* backToMenuButton;
    Engine::Image* background;

    void BackToMenuOnClick();
public:
    explicit WinScreen() = default;
    void Initialize() override;
    void Terminate() override;
};

#endif // WIN_SCREEN_HPP
