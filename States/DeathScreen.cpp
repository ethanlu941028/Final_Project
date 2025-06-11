#include "DeathScreen.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ImageButton.hpp"

void DeathScreen::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    background = new Engine::Image("lose/losescreen.png", 0, 0, w, h);
    AddNewObject(background);

    backToMenuButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 450, 400, 100);
    backToMenuButton->SetOnClickCallback(std::bind(&DeathScreen::BackToMenuOnClick, this));
    AddNewControlObject(backToMenuButton);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH / 2 + 500, 0, 0, 0, 255, 0.5, 0.5));
}

void DeathScreen::Terminate() {
    gameOverLabel = nullptr;
    backToMenuButton = nullptr;
    IScene::Terminate();
}

void DeathScreen::BackToMenuOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("title");
}

void DeathScreen::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode); // 如果基底類別有其他處理
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        Engine::GameEngine::GetInstance().ChangeScene("title");
    }
}
