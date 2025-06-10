#include "PauseMenu.hpp"

#include <iostream>

#include "Gameplay.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ImageButton.hpp"
#include "Engine/Group.hpp"


void PauseMenu::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;

    resumeButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, h / 2 - 100, 400, 100);
    resumeButton->SetOnClickCallback(std::bind(&PauseMenu::ResumeOnClick, this));
    AddNewControlObject(resumeButton);
    AddNewObject(new Engine::Label("Resume", "pirulen.ttf", 48, halfW, h / 2 - 50, 0, 0, 0, 255, 0.5, 0.5));

    exitButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, h / 2 + 100, 400, 100);
    exitButton->SetOnClickCallback(std::bind(&PauseMenu::ExitOnClick, this));
    AddNewControlObject(exitButton);
    AddNewObject(new Engine::Label("Exit", "pirulen.ttf", 48, halfW, h / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));
}

void PauseMenu::Terminate() {
    std::cout << "PauseMenu::Terminate called." << std::endl;
    resumeButton = nullptr;
    exitButton = nullptr;
    ClearObjects();
    IScene::Terminate();
}

void PauseMenu::ResumeOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("play"); // 返回 Gameplay
}

void PauseMenu::ExitOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("title");
}

void PauseMenu::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode); // 如果基底類別有其他處理
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }
}