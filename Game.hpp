#ifndef GAME_HPP
#define GAME_HPP

#pragma once
#include <memory>
#include <allegro5/allegro.h>
#include "Engine/IScene.hpp"
class IScene;

class Game {
public:
    Game();
    ~Game();
    void Run();

private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* eventQueue;
    ALLEGRO_TIMER* timer;
    ALLEGRO_BITMAP* icon;

    bool running;
    bool redraw;
    double lastTime = 0;

    std::unique_ptr<Engine::IScene> currentScene;

    void InitAllegro();
    void ProcessEvents();
    void Update();
    void Draw();
    void ChangeScene(Engine::IScene* newScene); // 接收 new 出來的場景
};

#endif //GAME_HPP
