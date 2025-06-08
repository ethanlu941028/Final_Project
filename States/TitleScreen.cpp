#include "TitleScreen.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include <iostream>

void TitleScreen::Initialize() {

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;


    // Start 按鈕
    startButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 200, 400, 100);
    //startButton->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(startButton);
    AddNewObject(new Engine::Label("Play", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 0, 0, 0, 255, 0.5, 0.5));

    // Exit 按鈕
    /*
    exitButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2, 400, 100);
    exitButton->SetOnClickCallback([]() {Engine::GameEngine::GetInstance().Close();});
    AddNewControlObject(exitButton);
    */

}

void TitleScreen::Terminate() {
    startButton = nullptr;
    exitButton = nullptr;
    IScene::Terminate();  // 呼叫基底釋放控制元件
}

void TitleScreen::OnMouseDown(int button, int mx, int my) {
    IScene::OnMouseDown(button, mx, my);  // 呼叫基底讓控制元件正常運作
}

void TitleScreen::OnEvent(ALLEGRO_EVENT& event) {
    // 根據需要處理輸入事件，例如：
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            Engine::GameEngine::GetInstance().Close(); // 按 ESC 退出遊戲
        }
    }
    IScene::OnEvent(event);  // 確保基底類也收到事件
}
