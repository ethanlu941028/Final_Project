#include "Gameplay.hpp"

#include <iostream>
#include <ostream>
#include <iomanip>
#include <sstream>

#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Entities/Player.hpp"

void Gameplay::Initialize() {
    score = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    background = new Engine::Image("play/floor.png", 0, 0, w, h);
    AddNewObject(background);

    scoreLabel = new Engine::Label("Score: 0", "pirulen.ttf", 24, 10, 10, 0, 0, 255, 255);
    AddNewObject(scoreLabel);

    pauseButton = new Engine::ImageButton("play/target-invalid.png","play/dirt.png", w - 70, 10, 64, 64);
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
    score += deltaTime * 60;

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << "Score: " << ((score*4) / 100.0f);
    scoreLabel->Text = stream.str();

    player->Update(deltaTime);
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
        player->Jump();
    }
}
