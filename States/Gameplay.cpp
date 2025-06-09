#include "Gameplay.hpp"

#include <iostream>
#include <ostream>

#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Entities/Player.hpp"  // 假設你有這個

void Gameplay::Initialize() {
    score = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    background = new Engine::Image("play/floor.png", 0, 0, w, h);
    AddNewObject(background);

    scoreLabel = new Engine::Label("Score: 0", "pirulen.ttf", 24, 10, 10, 0, 0, 255, 255);
    AddNewObject(scoreLabel);

    pauseButton = new Engine::ImageButton("play/target-invalid.png","stage-select/floor.png", w - 70, 10, 64, 64);
    pauseButton->SetOnClickCallback(std::bind(&Gameplay::PauseOnClick, this));
    AddNewControlObject(pauseButton);

    // 初始化玩家
    player = new Player(400,500);
    AddNewObject(player);
}

void Gameplay::Terminate() {
    background = nullptr;
    scoreLabel = nullptr;
    pauseButton = nullptr;
    //delete player;
    //player = nullptr;
    IScene::Terminate();
}

void Gameplay::Update(float deltaTime) {
    score += static_cast<int>(deltaTime * 60);
    scoreLabel->Text = "Score: " + std::to_string(score);

    // 檢查血量
    CheckPlayerHealth();
}

void Gameplay::CheckPlayerHealth() {
    if (player && player->GetHP() <= 0) {
        Engine::GameEngine::GetInstance().ChangeScene("death");
    }
}


void Gameplay::PauseOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("pause");
}

void Gameplay::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode); // 如果基底類別有其他處理

    if (keyCode == ALLEGRO_KEY_SPACE) {
        std::cout<<"jump"<<std::endl;
        player->Jump();
    }
}
