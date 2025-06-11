#ifndef TITLESCREEN_HPP
#define TITLESCREEN_HPP

#include <allegro5/allegro.h>
#include "Engine/IScene.hpp"
#include "UI/Component/ImageButton.hpp"

class TitleScreen final : public Engine::IScene {
private:
    Engine::ImageButton* startButton;
    Engine::ImageButton* exitButton;
    Engine::ImageButton* settingButton;

public:
    explicit TitleScreen() = default;
    void Initialize() override;
    void Terminate() override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnEvent(ALLEGRO_EVENT& event) override;
    void PlayOnClick(int stage);
    void SettingOnClick(int stage);
    void Draw() const;
};

#endif // TITLESCREEN_HPP
