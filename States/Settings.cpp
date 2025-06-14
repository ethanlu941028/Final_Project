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

void Settings::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // Background image
    AddNewObject(new Engine::Image("stage-select/mainBG.png", 0, 0, w, h));

    Engine::ImageButton* btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&Settings::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    Slider* sliderBGM = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4);
    sliderBGM->SetOnValueChangedCallback(std::bind(&Settings::BGMSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderBGM);
    AddNewObject(new Engine::Label("BGM: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH - 50, 255, 255, 255, 255, 0.5, 0.5));

    bgmInstance = nullptr;
    sliderBGM->SetValue(AudioHelper::BGMVolume);
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

void Settings::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        BackOnClick();
    }
}
