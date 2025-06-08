#ifndef TITLESCREEN_HPP
#define TITLESCREEN_HPP

#include <allegro5/allegro.h>
#include "Engine/IScene.hpp"
#include "UI/Component/ImageButton.hpp"

class TitleScreen final : public Engine::IScene {
private:
    Engine::ImageButton* startButton;
    Engine::ImageButton* exitButton;

public:
    explicit TitleScreen() = default;
    void Initialize() override;
    void Terminate() override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnEvent(ALLEGRO_EVENT& event) override;
};

#endif // TITLESCREEN_HPP
