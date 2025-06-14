#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <memory>
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

namespace Engine {
    class ImageButton;
    class Label;
}

class Settings final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    Engine::ImageButton* hitboxToggle = nullptr;
    Engine::ImageButton* cheatToggle = nullptr;
    Engine::Label* hitboxLabel = nullptr;
    Engine::Label* cheatLabel = nullptr;

public:
    explicit Settings() = default;
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

#endif // SETTINGS_HPP
