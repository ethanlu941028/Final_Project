#ifndef WIN_SCREEN_HPP
#define WIN_SCREEN_HPP

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>
#include <string>

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
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;

    void BackToMenuOnClick();

    std::string nameInput;
    Engine::Label* nameLabel;

    void SaveRecord(const std::string& name, int score);
    void OnSaveClick(int dummy);

public:
    explicit WinScreen() = default;
    void Initialize() override;
    void Terminate() override;
    void OnKeyDown(int keyCode) override;
    void Update(float deltaTime) override;
    void HandleEvent(const ALLEGRO_EVENT& event) ;  // to capture key events
};

#endif // WIN_SCREEN_HPP

