#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Gameplay.hpp"
#include "States/Settings.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "Utils/Config.hpp"

void Settings::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // Background image
    AddNewObject(new Engine::Image("stage-select/mainBG.png", 0, 0, w, h));

    // Title label
    AddNewObject(new Engine::Label("Settings", "pirulen.ttf", 72, halfW, halfH - 300, 255, 255, 255, 255, 0.5, 0.5));


    Engine::ImageButton* btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH + 150, 400, 100);
    btn->SetOnClickCallback(std::bind(&Settings::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH + 200, 0, 0, 0, 255, 0.5, 0.5));

    Slider* sliderBGM = new Slider(40 + halfW - 95, halfH -120 - 2, 190, 4);
    sliderBGM->SetOnValueChangedCallback(std::bind(&Settings::BGMSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderBGM);
    AddNewObject(new Engine::Label("BGM ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH -120, 255, 255, 255, 255, 0.5, 0.5));

    hitboxToggle = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW, halfH - 40, 80, 40);
    hitboxToggle->SetOnClickCallback(std::bind(&Settings::HitboxToggleOnClick, this));
    AddNewControlObject(hitboxToggle);
    AddNewObject(new Engine::Label("Hitbox", "pirulen.ttf", 28, halfW - 75, halfH - 40 + 20, 255, 255, 255, 255, 1, 0.5));
    hitboxLabel = new Engine::Label("", "pirulen.ttf", 24, halfW + 8, halfH - 40 + 20, 255, 255, 255, 255, 0.0, 0.5);
    AddNewObject(hitboxLabel);

    cheatToggle = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW, halfH + 40, 80, 40);
    cheatToggle->SetOnClickCallback(std::bind(&Settings::CheatToggleOnClick, this));
    AddNewControlObject(cheatToggle);
    AddNewObject(new Engine::Label("Cheat", "pirulen.ttf", 28, halfW - 75, halfH + 40 + 20, 255, 255, 255, 255, 1, 0.5));
    cheatLabel = new Engine::Label("", "pirulen.ttf", 24, halfW + 8, halfH + 40 + 20, 255, 255, 255, 255, 0.0, 0.5);
    AddNewObject(cheatLabel);

    bgmInstance = nullptr;
    sliderBGM->SetValue(AudioHelper::BGMVolume);
    hitboxLabel->Text = showHitboxSetting ? "ON" : "OFF";
    cheatLabel->Text = cheatModeSetting ? "ON" : "OFF";
}

void Settings::Terminate() {
    if (bgmInstance) {
        AudioHelper::StopSample(bgmInstance);
    }
    bgmInstance.reset();
    ClearObjects();
    IScene::Terminate();
}

void Settings::Draw() const {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    IScene::Draw();
}

void Settings::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("title");
}

void Settings::BGMSlideOnValueChanged(float value) {
    if (bgmInstance) {
        AudioHelper::ChangeSampleVolume(bgmInstance, value);
    }
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    Gameplay* gameplay = dynamic_cast<Gameplay*>(scene);
    if (gameplay && gameplay->bgmInstance) {
        al_set_sample_instance_gain(gameplay->bgmInstance.get(), value);
    }
    AudioHelper::BGMVolume = value;
}

void Settings::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}

void Settings::HitboxToggleOnClick() {
    showHitboxSetting = !showHitboxSetting;
    hitboxLabel->Text = showHitboxSetting ? "ON" : "OFF";
    showHitbox = showHitboxSetting;
}

void Settings::CheatToggleOnClick() {
    cheatModeSetting = !cheatModeSetting;
    cheatLabel->Text = cheatModeSetting ? "ON" : "OFF";
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    Gameplay* gameplay = dynamic_cast<Gameplay*>(scene);
    if (gameplay) {
        gameplay->cheatMode = cheatModeSetting;
    }
}

void Settings::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        BackOnClick();
    }
}
