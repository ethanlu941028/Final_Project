#include "Gameplay.hpp"

#include <iostream>
#include <ostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "Engine/GameEngine.hpp"
#include <vector>
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Entities/Player.hpp"
#include "Entities/Level.hpp"
#include "Entities/GroundTile.hpp"
#include "Entities/SpikeTile.hpp"
#include "Engine/Collider.hpp"
#include "Engine/Group.hpp"
#include "Utils/Config.hpp"
#include <iostream>

#include "Engine/AudioHelper.hpp"

const int Gameplay::MapWidth = 300, Gameplay::MapHeight = 15;


void Gameplay::Initialize() {
    if (initialized) return;
    initialized = true;
    showHitbox = false;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    background = new Engine::Image("play/Background.png", 0, 0, w, h);
    AddNewObject(background);
    AddNewObject(TileMapGroup = new Group());
    // Load level map
    std::string filename = std::string("Resource/level") + std::to_string(MapId) + ".txt";
    level = new Level(MapWidth, MapHeight, TileMapGroup);
    level->LoadFromFile(filename);
    level->InitializeView();


    score = 0;

    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);

    scoreLabel = new Engine::Label("Score: 0", "pirulen.ttf", 24, 10, 10, 0, 0, 255, 255);
    AddNewObject(scoreLabel);

    pauseButton = new Engine::ImageButton("play/target-invalid.png","stage-select/floor.png", w - 70, 10, 64, 64);
    pauseButton->SetOnClickCallback(std::bind(&Gameplay::PauseOnClick, this));
    AddNewControlObject(pauseButton);

    // Initialize player starting position based on visible tile rows
    int visibleRows = Engine::GameEngine::GetInstance().GetScreenHeight() / TILE_SIZE;
    player = new Player(400, (visibleRows - 7) * TILE_SIZE);
    AddNewObject(player);
}

void Gameplay::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    std::cout << "Gameplay::Terminate called." << std::endl;
    background = nullptr;
    scoreLabel = nullptr;
    pauseButton = nullptr;
    delete level;
    level = nullptr;
    initialized = false;
    //delete player;
    //player = nullptr;
    ClearObjects();

    IScene::Terminate();
}


void Gameplay::ReadMap() {
    std::string filename = std::string("Resource/map") + std::to_string(MapId) + ".txt";
    // Read map file line by line and pad missing data with empty tiles.
    std::ifstream fin(filename);
    if (!fin.is_open()) return;
    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth, TILE_EMPTY));
    std::string line;
    int row = 0;
    while (row < MapHeight && std::getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        for (int col = 0; col < MapWidth; ++col) {
            char c = (col < static_cast<int>(line.size())) ? line[col] : '0';
            bool num = (c == '1');
            mapState[row][col] = num ? TILE_WALL : TILE_EMPTY;
            if (num)
            TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", col * BlockSize, row * BlockSize, BlockSize, BlockSize));
            else
            TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", col * BlockSize, row * BlockSize, BlockSize, BlockSize));
        }
        ++row;
    }
    // Fill any remaining rows with empty tiles.
    for (; row < MapHeight; ++row) {
        for (int col = 0; col < MapWidth; ++col) {
            TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", col * BlockSize, row * BlockSize, BlockSize, BlockSize));
        }
    }
}

void Gameplay::Update(float deltaTime) {
    const float scrollSpeed = 500.0f;
    bool levelFinished = level && level->IsFinished();
    if (level && !levelFinished) {
        level->Scroll(deltaTime, scrollSpeed);
        score += deltaTime * 60;
    } else if (!level) {
        score += deltaTime * 60;
    }

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << "Score: " << ((score*4) / 100.0f);
    scoreLabel->Text = stream.str();

    Engine::Point prevPos = player->Position;
    float prevVel = player->GetVelocityY();
    float prevRot = player->rotationAngle;
    if (!levelFinished) player->Update(deltaTime);

    if (!levelFinished) {
        auto objects = TileMapGroup->GetObjects();
        for (auto* obj : objects) {
            if (auto* spike = dynamic_cast<SpikeTile*>(obj)) {
                auto pPolyArr = player->GetHitboxPoints();
                std::vector<Engine::Point> pPoly(pPolyArr.begin(), pPolyArr.end());
                auto bTL = spike->GetBaseHitboxTopLeft();
                auto bBR = spike->GetBaseHitboxBottomRight();
                auto tTL = spike->GetTopHitboxTopLeft();
                auto tBR = spike->GetTopHitboxBottomRight();
                if (Engine::Collider::IsPolygonOverlapRect(pPoly, bTL, bBR) ||
                    Engine::Collider::IsPolygonOverlapRect(pPoly, tTL, tBR)) {
                    player->SetHP(0);
                    break;
                }
                continue;
            }
            auto* g = dynamic_cast<GroundTile*>(obj);
            if (!g) continue;
            auto pPolyArr = player->GetHitboxPoints();
            std::vector<Engine::Point> pPoly(pPolyArr.begin(), pPolyArr.end());
            auto pTL = player->GetHitboxTopLeft();
            auto pBR = player->GetHitboxBottomRight();
            auto gTL = g->GetHitboxTopLeft();
            auto gBR = g->GetHitboxBottomRight();
            if (!Engine::Collider::IsPolygonOverlapRect(pPoly, gTL, gBR))
                continue;

            if (!player->upsideDown) {
                float prevBottom = Player::CalculateBottomY(prevPos, prevRot);
                float groundTop = gTL.y;
                float curBottom = player->GetBottomY();
                if (prevBottom <= groundTop && curBottom >= groundTop && prevVel >= 0) {
                    player->Land(groundTop);
                } else {
                    player->SetHP(0);
                }
            } else {
                float prevTop = Player::CalculateTopY(prevPos, prevRot);
                float groundBottom = gBR.y;
                float curTop = player->GetTopY();
                if (prevTop >= groundBottom && curTop <= groundBottom && prevVel <= 0) {
                    player->LandOnCeiling(groundBottom);
                } else {
                    player->SetHP(0);
                }
            }
            break;
        }
    }

    CheckPlayerHealth();

    if (levelFinished) {
        Engine::GameEngine::GetInstance().ChangeScene("win");
    }
}

void Gameplay::CheckPlayerHealth() {
    if (player && player->GetHP() <= 0) {
        Engine::GameEngine::GetInstance().ChangeScene("death");
    }
}


void Gameplay::PauseOnClick() {
    Engine::GameEngine::GetInstance().PushScene("pause");
}

void Gameplay::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode); // 如果基底類別有其他處理

    if (keyCode == ALLEGRO_KEY_SPACE) {
        player->Jump();
    }
    else if (keyCode == ALLEGRO_KEY_ESCAPE) {
        Engine::GameEngine::GetInstance().PushScene("pause");
    }
    else if (keyCode == ALLEGRO_KEY_0) {
        Engine::GameEngine::GetInstance().ChangeScene("death");
    }
    else if (keyCode == ALLEGRO_KEY_U) {
        player->Flip();
    }
    else if (keyCode == ALLEGRO_KEY_B) {
        showHitbox = !showHitbox;
    }
}

long long Gameplay::GetScore() {
    return (score * 4) / 100.0f;
}

/*
Gameplay &Gameplay::GetInstance() {
    // The classic way to lazy initialize a Singleton.
    static Gameplay instance;
    return instance;
}
*/