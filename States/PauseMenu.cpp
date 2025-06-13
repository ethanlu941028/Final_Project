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
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    Gameplay* gameplay = dynamic_cast<Gameplay*>(scene);
    if (gameplay && gameplay->bgmInstance &&
        al_get_sample_instance_playing(gameplay->bgmInstance.get())) {
        // Record the current position only if the music is playing.
        gameplay->bgmPausedPos = al_get_sample_instance_position(gameplay->bgmInstance.get());
        al_set_sample_instance_playing(gameplay->bgmInstance.get(), false); // 停止播放
    }

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    escButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, h / 2 - 200, 400, 100);
    escButton->SetOnClickCallback(std::bind(&PauseMenu::ResumeOnClick, this));
    AddNewControlObject(escButton);
    AddNewObject(new Engine::Label("BACK", "pirulen.ttf", 48, halfW, h / 2 - 150, 0, 0, 0, 255, 0.5, 0.5));

    restartButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, h / 2 -60, 400, 100);
    restartButton->SetOnClickCallback(std::bind(&PauseMenu::RestartOnClick, this));
    AddNewControlObject(restartButton);
    AddNewObject(new Engine::Label("Restart", "pirulen.ttf", 48, halfW, h / 2 + -10, 0, 0, 0, 255, 0.5, 0.5));

    settingButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, h / 2 + +80, 400, 100);
    settingButton->SetOnClickCallback(std::bind(&PauseMenu::SettingsOnClick, this));
    AddNewControlObject(settingButton);
    AddNewObject(new Engine::Label("Setting", "pirulen.ttf", 48, halfW, h / 2 + 130, 0, 0, 0, 255, 0.5, 0.5));

    quitButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, h / 2 + 220, 400, 100);
    quitButton->SetOnClickCallback(std::bind(&PauseMenu::ExitOnClick, this));
    AddNewControlObject(quitButton);
    AddNewObject(new Engine::Label("Quit", "pirulen.ttf", 48, halfW, h / 2 + 270, 0, 0, 0, 255, 0.5, 0.5));

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
    // Draw a translucent overlay so the gameplay background stays visible.
    al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0, 0, 0, 128));
    IScene::Draw();
}

void PauseMenu::ResumeOnClick() {
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    Gameplay* gameplay = dynamic_cast<Gameplay*>(scene);
    if (gameplay && gameplay->bgmInstance) {
        gameplay->isPaused = false; // 恢復遊戲狀態
        al_set_sample_instance_position(gameplay->bgmInstance.get(), gameplay->bgmPausedPos); // 恢復位置
        al_set_sample_instance_playing(gameplay->bgmInstance.get(), true); // 恢復 BGM
    }
    Engine::GameEngine::GetInstance().ChangeScene("play"); // 返回 Gameplay
}

void PauseMenu::RestartOnClick() {
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    Gameplay* gameplay = dynamic_cast<Gameplay*>(scene);
    if (gameplay) {
        gameplay->initialized = false; // 或 gameplay->Terminate();
    }
    Engine::GameEngine::GetInstance().ChangeScene("play");
}

void PauseMenu::SettingsOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("setting");
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
        ResumeOnClick();
    } else if (keyCode == ALLEGRO_KEY_Q) {
        PauseMenu::ExitOnClick();
    }
}

void PauseMenu::BGMSlideOnValueChanged(float value) {
    AudioHelper::BGMVolume = value;
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    Gameplay* gameplay = dynamic_cast<Gameplay*>(scene);
    if (gameplay && gameplay->bgmInstance) {
        al_set_sample_instance_gain(gameplay->bgmInstance.get(), value);
    }

}
void PauseMenu::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
