#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "Gameplay.hpp"
#include "States/SettingScreen.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "States/Gameplay.hpp"

void SettingScreen::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&SettingScreen::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    Slider *sliderBGM, *sliderSFX;
    sliderBGM = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4);
    sliderBGM->SetOnValueChangedCallback(std::bind(&SettingScreen::BGMSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderBGM);
    AddNewObject(new Engine::Label("BGM: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH - 50, 255, 255, 255, 255, 0.5, 0.5));
    sliderSFX = new Slider(40 + halfW - 95, halfH + 50 - 2, 190, 4);
    sliderSFX->SetOnValueChangedCallback(std::bind(&SettingScreen::SFXSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderSFX);
    AddNewObject(new Engine::Label("SFX: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH + 50, 255, 255, 255, 255, 0.5, 0.5));
    // No BGM is played in settings.
    bgmInstance = nullptr;
    sliderBGM->SetValue(AudioHelper::BGMVolume);
    sliderSFX->SetValue(AudioHelper::SFXVolume);
}
void SettingScreen::Terminate() {
    if (bgmInstance) {
        AudioHelper::StopSample(bgmInstance);
    }
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    ClearObjects();
    IScene::Terminate();
}

void SettingScreen::Draw() const {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0, 0, 0, 128));

    // 再呼叫基底的Draw，繼續畫控制元件等
    IScene::Draw();
}

void SettingScreen::BackOnClick(int stage) {
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    Gameplay* gameplay = dynamic_cast<Gameplay*>(scene);
    if (gameplay && gameplay->isPaused) {
        Engine::GameEngine::GetInstance().ChangeScene("pause");
    }
    else {
        Engine::GameEngine::GetInstance().ChangeScene("title");
    }

}


void SettingScreen::BGMSlideOnValueChanged(float value) {
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
void SettingScreen::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}

void SettingScreen::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode); // 如果基底類別有其他處理
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        SettingScreen::BackOnClick(0);
    }
}