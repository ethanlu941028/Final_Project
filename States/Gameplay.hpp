#ifndef PLAY_SCENE_GAMEPLAY_HPP
#define PLAY_SCENE_GAMEPLAY_HPP

#include <allegro5/allegro.h>
#include "Engine/IScene.hpp"
#include "Utils/Config.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <allegro5/allegro_audio.h>


namespace Engine {
    class Image;
    class Label;
    class Group;
}

class Player;  // 前向宣告
class Level;  // forward declaration for map handling
class JumpOrb;
class FlipOrb;

class Gameplay : public Engine::IScene {
private:
    Engine::Image* background;
    Engine::Label* scoreLabel;
    Engine::Label* progressLabel; // shows map completion percentage


    enum TileType {
        TILE_WALL,
        TILE_EMPTY
    };

    int score;
    Player* player;
    Level* level = nullptr;

    class JumpOrb* overlappingJumpOrb = nullptr;
    class FlipOrb* overlappingFlipOrb = nullptr;

    bool playerDead = false;
    float deadTimer = 0.0f;
    bool deathSoundPlayed = false;

    bool victoryCutscene = false;
    float victoryTimer = 0.0f;
    float flightSpeed = 0.0f;

    bool jumpRequested = false;
    float jumpBufferTimer = 0.0f;
    static constexpr float JumpBufferDuration = 0.1f;

    void CheckPlayerHealth();

public:
    int MapId;
    unsigned int bgmPausedPos = 0;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    static const std::vector<Engine::Point> directions;
    static const int MapWidth, MapHeight;
    static constexpr int BlockSize = TILE_SIZE;
    std::vector<std::vector<TileType>> mapState;
    std::vector<std::vector<int>> mapDistance;
    Engine::Group* TileMapGroup;
    bool initialized = false;
    static bool isPaused;
    explicit Gameplay() = default;
    void Initialize() override;
    void ReadMap();
    void Terminate() override;
    void Update(float deltaTime) override;
    void OnKeyDown(int keyCode) override;
    long long GetScore();
    //static Gameplay &GetInstance();
    //void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    //void OnEvent(ALLEGRO_EVENT& event) override;
};

#endif // PLAY_SCENE_GAMEPLAY_HPP
