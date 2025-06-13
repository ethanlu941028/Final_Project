#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP
#include <memory>
#include <iostream>
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>


namespace Engine {
    class Label;
    class ImageButton;
}

class PauseMenu : public Engine::IScene {
private:
    Engine::ImageButton* escButton;
    Engine::ImageButton* quitButton;
    Engine::ImageButton* restartButton;
    Engine::ImageButton* settingButton;
public:
    void Initialize() override;
    void Terminate() override;
    void ResumeOnClick();
    void ExitOnClick();
    void RestartOnClick();
    void SettingsOnClick();
    void OnKeyDown(int keyCode) override;
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
    void Draw() const;

};

#endif // PAUSEMENU_HPP
