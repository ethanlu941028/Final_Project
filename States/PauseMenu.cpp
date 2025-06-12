#include "PauseMenu.hpp"
#include <allegro5/allegro_audio.h>
#include <iostream>
#include <functional>
#include <memory>
#include <string>
#include <allegro5/allegro_primitives.h>
#include <map>

#include "Engine/AudioHelper.hpp"
#include "Engine/Point.hpp"
#include "Gameplay.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ImageButton.hpp"
#include "Engine/Group.hpp"
#include "UI/Component/Slider.hpp"
#include "States/Gameplay.hpp"


void PauseMenu::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    escButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, h / 2 - 200, 400, 100);
    escButton->SetOnClickCallback(std::bind(&PauseMenu::ResumeOnClick, this));
    AddNewControlObject(escButton);
    AddNewObject(new Engine::Label("ESC", "pirulen.ttf", 48, halfW, h / 2 - 150, 0, 0, 0, 255, 0.5, 0.5));

    quitButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, h / 2 + 100, 400, 100);
    quitButton->SetOnClickCallback(std::bind(&PauseMenu::ExitOnClick, this));
    AddNewControlObject(quitButton);
    AddNewObject(new Engine::Label("Quit", "pirulen.ttf", 48, halfW, h / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));

    Slider *sliderBGM, *sliderSFX;
    sliderBGM = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4);
    sliderBGM->SetOnValueChangedCallback(std::bind(&PauseMenu::BGMSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderBGM);
    AddNewObject(new Engine::Label("BGM: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH - 50, 255, 255, 255, 255, 0.5, 0.5));
    sliderSFX = new Slider(40 + halfW - 95, halfH + 50 - 2, 190, 4);
    sliderSFX->SetOnValueChangedCallback(std::bind(&PauseMenu::SFXSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderSFX);
    AddNewObject(new Engine::Label("SFX: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH + 50, 255, 255, 255, 255, 0.5, 0.5));
    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    sliderBGM->SetValue(AudioHelper::BGMVolume);
    sliderSFX->SetValue(AudioHelper::SFXVolume);
}

void PauseMenu::Terminate() {
    std::cout << "PauseMenu::Terminate called." << std::endl;
    escButton = nullptr;
    quitButton = nullptr;
    ClearObjects();
    IScene::Terminate();
}

void PauseMenu::Draw() const{
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    // 清空畫面背景，這邊用黑色，你可以改成你喜歡的顏色
    al_draw_filled_rectangle(halfW - 200, h / 2 - 50 - 30, halfW - 200 + 400, h / 2 - 50 + 100 + 30, al_map_rgba(0, 0, 0, 128));

    // 再呼叫基底的Draw，繼續畫控制元件等
    IScene::Draw();
}

void PauseMenu::ResumeOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("play"); // 返回 Gameplay
}

void PauseMenu::ExitOnClick() {
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    Gameplay* gameplay = dynamic_cast<Gameplay*>(scene);
    if (gameplay) {
        gameplay->initialized = false; // 或 gameplay->Terminate();
    }
    Engine::GameEngine::GetInstance().ChangeScene("title");
}


void PauseMenu::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode); // 如果基底類別有其他處理
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }
    else if (keyCode == ALLEGRO_KEY_Q) {
        PauseMenu::ExitOnClick();
    }
}

void PauseMenu::BGMSlideOnValueChanged(float value) {
    AudioHelper::BGMVolume = value;
}
void PauseMenu::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
