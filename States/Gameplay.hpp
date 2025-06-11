#ifndef PLAY_SCENE_GAMEPLAY_HPP
#define PLAY_SCENE_GAMEPLAY_HPP

#include <allegro5/allegro.h>
#include "Engine/IScene.hpp"
#include <iostream>


namespace Engine {
    class Image;
    class Label;
    class ImageButton;
}

class Player;  // 前向宣告

class Gameplay : public Engine::IScene {
private:
    Engine::Image* background;
    Engine::Label* scoreLabel;
    Engine::ImageButton* pauseButton;

    enum TileType {
        TILE_WALL,
        TILE_EMPTY
    };

    int score;
    Player* player;


    void PauseOnClick();
    void CheckPlayerHealth();

public:
    int MapId;
    static const std::vector<Engine::Point> directions;
    static const int MapWidth, MapHeight;
    static const int BlockSize;
    std::vector<std::vector<TileType>> mapState;
    std::vector<std::vector<int>> mapDistance;
    Group *TileMapGroup;
    bool initialized = false;
    explicit Gameplay() = default;
    void Initialize() override;
    void ReadMap();
    void Terminate() override;
    void Update(float deltaTime) override;
    void OnKeyDown(int keyCode) override;
    //void Draw() const override;
    //void OnMouseDown(int button, int mx, int my) override;
    //void OnEvent(ALLEGRO_EVENT& event) override;
};

#endif // PLAY_SCENE_GAMEPLAY_HPP
