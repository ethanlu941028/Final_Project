#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Gameplay.hpp"
#include "States/PauseSettings.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "Utils/Config.hpp"

void PauseSettings::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // Title label
    AddNewObject(new Engine::Label("Settings", "pirulen.ttf", 72, halfW, halfH - 300, 255, 255, 255, 255, 0.5, 0.5));


    Engine::ImageButton* btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH + 150, 400, 100);
    btn->SetOnClickCallback(std::bind(&PauseSettings::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH + 200, 0, 0, 0, 255, 0.5, 0.5));
    
    AddNewObject(new Engine::Label("BGM ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH -120, 255, 255, 255, 255, 0.5, 0.5));
    Slider* sliderBGM = new Slider(40 + halfW - 95, halfH -120 - 2, 190, 4);
    sliderBGM->SetOnValueChangedCallback(std::bind(&PauseSettings::BGMSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderBGM);
    
    hitboxToggle = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW, halfH - 40, 80, 40);
    hitboxToggle->SetOnClickCallback(std::bind(&PauseSettings::HitboxToggleOnClick, this));
    AddNewControlObject(hitboxToggle);
    AddNewObject(new Engine::Label("Hitbox", "pirulen.ttf", 28, halfW - 75, halfH - 40 + 20, 255, 255, 255, 255, 1, 0.5));
    hitboxLabel = new Engine::Label("", "pirulen.ttf", 24, halfW + 8, halfH - 40 + 20, 255, 255, 255, 255, 0.0, 0.5);
    AddNewObject(hitboxLabel);

    cheatToggle = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW, halfH + 40, 80, 40);
    cheatToggle->SetOnClickCallback(std::bind(&PauseSettings::CheatToggleOnClick, this));
    AddNewControlObject(cheatToggle);
    AddNewObject(new Engine::Label("Cheat", "pirulen.ttf", 28, halfW - 75, halfH + 40 + 20, 255, 255, 255, 255, 1, 0.5));
    cheatLabel = new Engine::Label("", "pirulen.ttf", 24, halfW + 8, halfH + 40 + 20, 255, 255, 255, 255, 0.0, 0.5);
    AddNewObject(cheatLabel);

    bgmInstance = nullptr;
    sliderBGM->SetValue(AudioHelper::BGMVolume);
    hitboxLabel->Text = showHitboxSetting ? "ON" : "OFF";
    cheatLabel->Text = cheatModeSetting ? "ON" : "OFF";
}

void PauseSettings::Terminate() {
    if (bgmInstance) {
        AudioHelper::StopSample(bgmInstance);
    }
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    ClearObjects();
    IScene::Terminate();
}

void PauseSettings::Draw() const {
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    if (scene) scene->Draw();
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0, 0, 0, 160));
    IScene::Draw();
}

void PauseSettings::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("pause");
}

void PauseSettings::BGMSlideOnValueChanged(float value) {
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

void PauseSettings::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}

void PauseSettings::HitboxToggleOnClick() {
    showHitboxSetting = !showHitboxSetting;
    hitboxLabel->Text = showHitboxSetting ? "ON" : "OFF";
    showHitbox = showHitboxSetting;
}

void PauseSettings::CheatToggleOnClick() {
    cheatModeSetting = !cheatModeSetting;
    cheatLabel->Text = cheatModeSetting ? "ON" : "OFF";
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    Gameplay* gameplay = dynamic_cast<Gameplay*>(scene);
    if (gameplay) {
        gameplay->cheatMode = cheatModeSetting;
    }
}

void PauseSettings::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        BackOnClick();
    }
}