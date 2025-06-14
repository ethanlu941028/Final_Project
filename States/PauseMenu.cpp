#include "PauseMenu.hpp"
#include <allegro5/allegro_audio.h>
#include <iostream>
#include <functional>
#include <memory>
#include <string>
#include <allegro5/allegro_primitives.h>

#include "Engine/AudioHelper.hpp"
#include "Engine/Point.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Slider.hpp"
#include "States/Gameplay.hpp"

void PauseMenu::Initialize() {
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    Gameplay* gameplay = dynamic_cast<Gameplay*>(scene);
    if (gameplay && gameplay->bgmInstance &&
        al_get_sample_instance_playing(gameplay->bgmInstance.get())) {
        gameplay->bgmPausedPos = al_get_sample_instance_position(gameplay->bgmInstance.get());
        al_set_sample_instance_playing(gameplay->bgmInstance.get(), false);
    }
    ShowPauseOptions();
}

void PauseMenu::ShowPauseOptions() {
    Clear();
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // Title label
    AddNewObject(new Engine::Label("Game Paused", "pirulen.ttf", 72, halfW, halfH - 300, 255, 255, 255, 255, 0.5, 0.5));

    escButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH - 200, 400, 100);
    escButton->SetOnClickCallback(std::bind(&PauseMenu::ResumeOnClick, this));
    AddNewControlObject(escButton);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH - 150, 0, 0, 0, 255, 0.5, 0.5));

    restartButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH - 50, 400, 100);
    restartButton->SetOnClickCallback(std::bind(&PauseMenu::RestartOnClick, this));
    AddNewControlObject(restartButton);
    AddNewObject(new Engine::Label("Restart", "pirulen.ttf", 48, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5));

    settingButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH + 100, 400, 100);
    settingButton->SetOnClickCallback(std::bind(&PauseMenu::SettingsOnClick, this));
    AddNewControlObject(settingButton);
    AddNewObject(new Engine::Label("Settings", "pirulen.ttf", 48, halfW, halfH + 150, 0, 0, 0, 255, 0.5, 0.5));

    quitButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH + 250, 400, 100);
    quitButton->SetOnClickCallback(std::bind(&PauseMenu::ExitOnClick, this));
    AddNewControlObject(quitButton);
    AddNewObject(new Engine::Label("Quit", "pirulen.ttf", 48, halfW, halfH + 300, 0, 0, 0, 255, 0.5, 0.5));
}

void PauseMenu::Terminate() {
    escButton = nullptr;
    quitButton = nullptr;
    Clear();
    IScene::Terminate();
}

void PauseMenu::Draw() const {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    al_draw_filled_rectangle(0, 0, w, h, al_map_rgba(0, 0, 0, 128));
    IScene::Draw();
}

void PauseMenu::ResumeOnClick() {
    Engine::IScene* scene = Engine::GameEngine::GetInstance().GetScene("play");
    Gameplay* gameplay = dynamic_cast<Gameplay*>(scene);
    if (gameplay && gameplay->bgmInstance) {
        gameplay->isPaused = false;
        al_set_sample_instance_position(gameplay->bgmInstance.get(), gameplay->bgmPausedPos);
        al_set_sample_instance_playing(gameplay->bgmInstance.get(), true);
    }
    Engine::GameEngine::GetInstance().ChangeScene("play");
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
    Engine::GameEngine::GetInstance().ChangeScene("pause-setting");
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
        ExitOnClick();
    }
}
