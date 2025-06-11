#include "TitleScreen.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include <iostream>
#include "States/Gameplay.hpp"
#include "Gameplay.hpp"

void TitleScreen::Initialize() {
    ClearObjects();
    //Gameplay::initialized = false;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;


    // Start 按鈕
    startButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2, 400, 100);
    startButton->SetOnClickCallback(std::bind(&TitleScreen::PlayOnClick, this, 1));
    AddNewControlObject(startButton);
    AddNewObject(new Engine::Label("Play", "pirulen.ttf", 48, halfW, halfH / 2 + 50, 0, 0, 0, 255, 0.5, 0.5));

    //Setting按鈕
    settingButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 +200, 400, 100);
    settingButton->SetOnClickCallback(std::bind(&TitleScreen::SettingOnClick, this, 2));
    AddNewControlObject(settingButton);
    AddNewObject(new Engine::Label("Settings", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 0, 0, 0, 255, 0.5, 0.5));

    // Exit 按鈕
    exitButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 400, 400, 100);
    exitButton->SetOnClickCallback([]() {Engine::GameEngine::GetInstance().Close();});
    AddNewControlObject(exitButton);
    AddNewObject(new Engine::Label("Exit", "pirulen.ttf", 48, halfW, halfH / 2 + 450, 0, 0, 0, 255, 0.5, 0.5));


}

void TitleScreen::Terminate() {
    Clear();
    startButton = nullptr;
    exitButton = nullptr;
    std::cout<<"TitleScreen Terminate"<<std::endl;
    IScene::Terminate();  // 呼叫基底釋放控制元件
}

void TitleScreen::Draw() const {
    // 清空畫面背景，這邊用黑色，你可以改成你喜歡的顏色
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // 再呼叫基底的Draw，繼續畫控制元件等
    IScene::Draw();
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

void TitleScreen::PlayOnClick(int stage) {
    // 這裡你可以根據選擇的關卡（stage）傳值
    // 若沒用到可以忽略參數
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void TitleScreen::SettingOnClick(int stage) {
    // 這裡你可以根據選擇的關卡（stage）傳值
    // 若沒用到可以忽略參數
    Engine::GameEngine::GetInstance().ChangeScene("setting");
}