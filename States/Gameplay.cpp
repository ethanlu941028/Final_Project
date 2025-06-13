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
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Entities/Player.hpp"
#include "Entities/Level.hpp"
#include "Entities/GroundTile.hpp"
#include "Entities/SpikeTile.hpp"
#include "Entities/FlipOrb.hpp"
#include "Entities/JumpOrb.hpp"
#include "Engine/Collider.hpp"
#include "Engine/Group.hpp"
#include "Utils/Config.hpp"
#include <iostream>

#include "Engine/AudioHelper.hpp"

const int Gameplay::MapWidth = 500, Gameplay::MapHeight = 15;

bool Gameplay::isPaused = false;

void Gameplay::Initialize() {
    if (initialized) return;
    initialized = true;
    isPaused = false;
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
    playerDead = false;
    deadTimer = 0.0f;
    deathSoundPlayed = false;

    bgmInstance = AudioHelper::PlaySample("BGM1.ogg", true, AudioHelper::BGMVolume);

    scoreLabel = new Engine::Label("Score: 0", "pirulen.ttf", 24, 10, 5, 255, 255, 255, 255);
    AddNewObject(scoreLabel);

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
    if (playerDead) {
        if (!deathSoundPlayed) {
            deathSoundPlayed = true;
        }
        deadTimer -= deltaTime;
        if (deadTimer <= 0.0f) {
            Engine::GameEngine::GetInstance().ChangeScene("play");
        }
        return;
    }

    const float scrollSpeed = 600.0f;
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
            bool circleOverlap = Engine::Collider::IsCircleOverlapRect(player->Position, player->GetGroundRadius(), gTL, gBR);

            if (!player->upsideDown) {
                float prevBottom = prevPos.y + Player::HitboxSize / 2.0f;
                float groundTop = gTL.y;
                float curBottom = player->Position.y + Player::HitboxSize / 2.0f;
                if (prevBottom <= groundTop && curBottom >= groundTop && prevVel >= 0) {
                    player->Land(groundTop);
                } else if (circleOverlap) {
                    player->SetHP(0);
                }
            } else {
                float prevTop = prevPos.y - Player::HitboxSize / 2.0f;
                float groundBottom = gBR.y;
                float curTop = player->Position.y - Player::HitboxSize / 2.0f;
                if (prevTop >= groundBottom && curTop <= groundBottom && prevVel <= 0) {
                    player->LandOnCeiling(groundBottom);
                } else if (circleOverlap) {
                    player->SetHP(0);
                }
            }
            break;
        }
    }

    if (!levelFinished) {
        overlappingFlipOrb = nullptr;
        auto objects = TileMapGroup->GetObjects();
        for (auto* obj : objects) {
            auto* orb = dynamic_cast<FlipOrb*>(obj);
            if (!orb) continue;
            auto pPolyArr = player->GetHitboxPoints();
            std::vector<Engine::Point> pPoly(pPolyArr.begin(), pPolyArr.end());
            if (Engine::Collider::IsCircleOverlapPolygon(orb->Position, orb->GetRadius(), pPoly)) {
                overlappingFlipOrb = orb;
                break;
            }
        }
    }

    if (!levelFinished) {
        overlappingJumpOrb = nullptr;
        auto objects = TileMapGroup->GetObjects();
        for (auto* obj : objects) {
            auto* orb = dynamic_cast<JumpOrb*>(obj);
            if (!orb) continue;
            auto pPolyArr = player->GetHitboxPoints();
            std::vector<Engine::Point> pPoly(pPolyArr.begin(), pPolyArr.end());
            if (Engine::Collider::IsCircleOverlapPolygon(orb->Position, orb->GetRadius(), pPoly)) {
                overlappingJumpOrb = orb;
                break;
            }
        }
    }

    CheckPlayerHealth();

    if (levelFinished) {
        Engine::GameEngine::GetInstance().ChangeScene("win");
    }
}

void Gameplay::CheckPlayerHealth() {
    if (player && player->GetHP() <= 0 && !playerDead) {
        playerDead = true;
        deadTimer = 2.0f; // 顯示時間為 2 秒
        deathSoundPlayed = false;
        showHitbox = true; // 顯示 hitbox
        AudioHelper::StopSample(bgmInstance);
        bgmInstance = AudioHelper::PlaySample("lose.ogg", true, AudioHelper::BGMVolume);
    }
}

void Gameplay::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode); // 如果基底類別有其他處理
    
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        isPaused = true;
        Engine::GameEngine::GetInstance().PushScene("pause");
    }
    else if (keyCode == ALLEGRO_KEY_0) {
        Engine::GameEngine::GetInstance().ChangeScene("death");
    }
    else if (keyCode == ALLEGRO_KEY_B) {
        showHitbox = !showHitbox;
    }
    else if (keyCode == ALLEGRO_KEY_SPACE) {
        if (player && !isPaused) {
            if (overlappingJumpOrb) {
                player->OrbJump();
                TileMapGroup->RemoveObject(overlappingJumpOrb->GetObjectIterator());
                overlappingJumpOrb = nullptr;
            } else if (overlappingFlipOrb){
                player->Flip();
                TileMapGroup->RemoveObject(overlappingFlipOrb->GetObjectIterator());
                overlappingFlipOrb = nullptr;
            } else {
                player->Jump();
            }
        }
    }
}

void Gameplay::OnMouseDown(int button, int mx, int my) {
    IScene::OnMouseDown(button, mx, my);  // Propagate to UI controls
    if (button == 1 && player && !isPaused) {
        if (overlappingJumpOrb) {
            player->OrbJump();
            TileMapGroup->RemoveObject(overlappingJumpOrb->GetObjectIterator());
            overlappingJumpOrb = nullptr;
        } else if (overlappingFlipOrb) {
            player->Flip();
            TileMapGroup->RemoveObject(overlappingFlipOrb->GetObjectIterator());
            overlappingFlipOrb = nullptr;
        } else {
            player->Jump();
        }
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