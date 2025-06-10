#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include "Engine/IScene.hpp"

namespace Engine {
    class Label;
    class ImageButton;
}

class PauseMenu : public Engine::IScene {
private:
    Engine::ImageButton* resumeButton;
    Engine::ImageButton* exitButton;
public:
    void Initialize() override;
    void Terminate() override;
    void ResumeOnClick();
    void ExitOnClick();
    void OnKeyDown(int keyCode) override;
};

#endif // PAUSEMENU_HPP
