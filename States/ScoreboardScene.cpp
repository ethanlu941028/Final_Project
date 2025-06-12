#include "States/ScoreboardScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>


ScoreboardScene::ScoreboardScene()
    : currentPage(0), entriesPerPage(10) {}
ScoreboardScene::~ScoreboardScene() = default;

void ScoreboardScene::Initialize() {
    LoadScoreboard();
    SortEntries();
    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
    ChangingPage = false;
    ShowPage(0);
}

void ScoreboardScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance.reset();
    IScene::Terminate();
}

void ScoreboardScene::Draw() const {
    // 清空畫面背景，這邊用黑色，你可以改成你喜歡的顏色
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // 再呼叫基底的Draw，繼續畫控制元件等
    IScene::Draw();
}


void ScoreboardScene::LoadScoreboard() {
    entries.clear();
    std::ifstream file("Resource/scoreboard.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        ScoreEntry e;
        if (!(iss >> e.name >> e.score)) continue;
        // read optional datetime
        if (!(iss >> e.datetime)) {
            e.datetime.clear();
        }
        entries.push_back(e);
    }
}

void ScoreboardScene::SortEntries() {
    std::sort(entries.begin(), entries.end(), [](auto &a, auto &b) {
        return a.score > b.score;
    });
}

void ScoreboardScene::ShowPage(unsigned int page) {
    currentPage = page;
    ClearObjects();

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // Prev button
    auto* prevButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
        halfW - 650, halfH * 3 / 2 + 50, 400, 100);
    AddNewControlObject(prevButton);
    AddNewObject(new Engine::Label("Prev", "pirulen.ttf", 48,
        halfW - 450, halfH * 3 / 2 + 100, 0, 0, 0, 255, 0.5, 0.5));

    // Next button
    auto* nextButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
        halfW + 250, halfH * 3 / 2 + 50, 400, 100);
    AddNewControlObject(nextButton);
    AddNewObject(new Engine::Label("Next", "pirulen.ttf", 48,
        halfW + 450, halfH * 3 / 2 + 100, 0, 0, 0, 255, 0.5, 0.5));

    // Back button
    auto* backButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
        halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    AddNewControlObject(backButton);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48,
        halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    if (!ChangingPage){
        prevButton->SetOnClickCallback(std::bind(&ScoreboardScene::PrevPageOnClick, this, 0));
        nextButton->SetOnClickCallback(std::bind(&ScoreboardScene::NextPageOnClick, this, 0));
        backButton->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 1));

        ChangingPage = true;
    }

    // 顯示頁數

    std::string pageText = "Page " + std::to_string(currentPage + 1);
    AddNewObject(new Engine::Label(pageText, "pirulen.ttf", 48,120, 70, 0, 255, 0, 255, 0.5, 1.0));


    int start = page * entriesPerPage;
    int end = std::min(start + entriesPerPage, (unsigned int)entries.size());
    int y = 100;
    for (unsigned int i = start; i < end; ++i) {
        const auto &e = entries[i];
        // build display text with optional datetime
        std::string text = std::to_string(i + 1) + ". " + e.name + " - " + std::to_string(e.score);
        if (!e.datetime.empty()) {
            text += " (" + e.datetime + ")";
        }
        if (e.score == -87878787) AddNewObject(new Engine::Label(text, "pirulen.ttf", 36,halfW, y, 255, 0, 0, 255, 0.5, 0));
        else AddNewObject(new Engine::Label(text, "pirulen.ttf", 36,halfW, y, 0, 255, 0, 255, 0.5, 0));
        y += 50;
    }
}

void ScoreboardScene::PrevPageOnClick(int) {
    unsigned int maxPage = (entries.size() - 1) / entriesPerPage;
    if (currentPage > 0) ShowPage(currentPage - 1);
    else if (currentPage == 0) ShowPage(maxPage);
}

void ScoreboardScene::NextPageOnClick(int) {
    unsigned int maxPage = (entries.size() - 1) / entriesPerPage;
    if (currentPage < maxPage) ShowPage(currentPage + 1);
    else if (currentPage == maxPage) ShowPage(0);
};

void ScoreboardScene::BackOnClick(int) {
    Engine::GameEngine::GetInstance().ChangeScene("title");
}

void ScoreboardScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}

void ScoreboardScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}

void ScoreboardScene::SaveScoreboard() {
    std::ofstream file("Resource/scoreboard.txt");
    for (auto &e : entries) {
        file << e.name << " " << e.score;
        if (!e.datetime.empty()) file << " " << e.datetime;
        file << "\n";
    }
}

void ScoreboardScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode); // 如果基底類別有其他處理
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        Engine::GameEngine::GetInstance().ChangeScene("title");
    }
    else if (keyCode == ALLEGRO_KEY_LEFT) {
        unsigned int maxPage = (entries.size() - 1) / entriesPerPage;
        if (currentPage > 0) ShowPage(currentPage - 1);
        else if (currentPage == 0) ShowPage(maxPage);
    }
    else if (keyCode == ALLEGRO_KEY_RIGHT) {
        unsigned int maxPage = (entries.size() - 1) / entriesPerPage;
        if (currentPage < maxPage) ShowPage(currentPage + 1);
        else if (currentPage == maxPage) ShowPage(0);
    }
}