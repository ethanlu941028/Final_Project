#include "DeathScreen.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ImageButton.hpp"

void DeathScreen::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    gameOverLabel = new Engine::Label("Game Over", "pirulen.ttf", 72, w / 2, h / 3, 255, 0, 0, 255, 0.5, 0.5);
    AddNewObject(gameOverLabel);

    backToMenuButton = new Engine::ImageButton("resources/start.png", "resources/start-hover.png", w / 2 - 100, h / 2, 200, 60);
    backToMenuButton->SetOnClickCallback(std::bind(&DeathScreen::BackToMenuOnClick, this));
    AddNewControlObject(backToMenuButton);
}

void DeathScreen::Terminate() {
    gameOverLabel = nullptr;
    backToMenuButton = nullptr;
    IScene::Terminate();
}

void DeathScreen::BackToMenuOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("title");
}
