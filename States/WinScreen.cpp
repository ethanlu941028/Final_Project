#include <functional>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <iostream>
#include "Engine/AudioHelper.hpp"
#include "WinScreen.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ImageButton.hpp"
#include "Gameplay.hpp"

void WinScreen::Initialize() {
    ticks = 0;
    nameInput.clear();
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    background = new Engine::Image("win/winscreen.png", 0, 0, w, h);
    AddNewObject(background);

    nameLabel = new Engine::Label("_", "pirulen.ttf", 60,halfW, halfH, 255, 0, 0, 255, 0.5, 0.5);
    AddNewObject(nameLabel);

    backToMenuButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 450, 400, 100);
    backToMenuButton->SetOnClickCallback(std::bind(&WinScreen::BackToMenuOnClick, this));
    AddNewControlObject(backToMenuButton);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH / 2 + 500, 0, 0, 0, 255, 0.5, 0.5));
}

void WinScreen::Terminate() {
    gameOverLabel = nullptr;
    backToMenuButton = nullptr;
    nameLabel =nullptr;
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}

void WinScreen::BackToMenuOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("title");
}

void WinScreen::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<Gameplay*>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
        }
}

void WinScreen::OnSaveClick(int) {
    std::string name = nameInput.empty() ? "Anonymous" : nameInput;
    int score = dynamic_cast<Gameplay*>(Engine::GameEngine::GetInstance().GetScene("play"))->GetScore();
    SaveRecord(name, score);
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
}

void WinScreen::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode); // 如果基底類別有其他處理
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        Engine::GameEngine::GetInstance().ChangeScene("title");
    }
}

void WinScreen::SaveRecord(const std::string& name, int score) {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ofstream file("Resource/scoreboard.txt", std::ios::app);
    file << name << " " << score << " ";
    file << std::put_time(&tm, "%Y-%m-%d_%H:%M:%S") << "\n";
}

// Handle keyboard input directly
void WinScreen::HandleEvent(const ALLEGRO_EVENT& event) {
    if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
        if (event.keyboard.unichar >= 32 && event.keyboard.unichar <= 126 && nameInput.size() < 16) {
            nameInput += static_cast<char>(event.keyboard.unichar);
        } else if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && !nameInput.empty()) {
            nameInput.pop_back();
        } else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            std::string name = nameInput.empty() ? "Anonymous" : nameInput;
            int score = dynamic_cast<Gameplay*>(Engine::GameEngine::GetInstance().GetScene("play"))->GetScore();
            SaveRecord(name, score);
            Engine::GameEngine::GetInstance().ChangeScene("scoreboard");
        }
        // update display
        nameLabel->Text = nameInput + "_";
    }
}