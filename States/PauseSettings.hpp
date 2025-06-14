#ifndef PAUSE_SETTINGS_HPP
#define PAUSE_SETTINGS_HPP
#include <memory>
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

namespace Engine {
    class ImageButton;
    class Label;
}

class PauseSettings final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    Engine::ImageButton* hitboxToggle = nullptr;
    Engine::ImageButton* cheatToggle = nullptr;
    Engine::Label* hitboxLabel = nullptr;
    Engine::Label* cheatLabel = nullptr;
public:
    explicit PauseSettings() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick();
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
    void HitboxToggleOnClick();
    void CheatToggleOnClick();
    void Draw() const override;
    void OnKeyDown(int keyCode) override;
};

#endif // PAUSE_SETTINGS_HPP