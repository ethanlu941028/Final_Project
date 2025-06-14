#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <memory>
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class Settings final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit Settings() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick();
    void BGMSlideOnValueChanged(float value);
    void Draw() const override;
    void OnKeyDown(int keyCode) override;
};

#endif // SETTINGS_HPP
