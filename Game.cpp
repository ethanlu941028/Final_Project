// Game.cpp

#include "Game.hpp"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Engine/IScene.hpp"
#include "States/TitleScreen.hpp"

Game::Game() {
    InitAllegro();
    currentScene = std::make_unique<TitleScreen>();
    running = true;
    redraw = false;
}

Game::~Game() {
    al_destroy_display(display);
    al_destroy_event_queue(eventQueue);
    al_destroy_timer(timer);
}

void Game::InitAllegro() {
    al_init();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10);
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    display = al_create_display(800, 600);
    timer = al_create_timer(1.0 / 60);
    eventQueue = al_create_event_queue();
    lastTime = al_get_time();  // 初始時間

    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));

    al_start_timer(timer);
}

void Game::Run() {
    while (running) {
        ProcessEvents();
        if (redraw && al_is_event_queue_empty(eventQueue)) {
            Update();
            Draw();
            redraw = false;
        }
    }
}

void Game::ProcessEvents() {
    ALLEGRO_EVENT ev;
    while (al_get_next_event(eventQueue, &ev)) {
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }
        currentScene->OnEvent(ev); // 轉交事件給目前的場景
    }
}

void Game::Update() {
    double currentTime = al_get_time();
    float deltaTime = static_cast<float>(currentTime - lastTime);
    lastTime = currentTime;
    currentScene->Update(deltaTime);
}

void Game::Draw() {
    al_clear_to_color(al_map_rgb(255, 255, 255));
    currentScene->Draw();
    al_flip_display();
}

void Game::ChangeScene(Engine::IScene* newScene) {
    currentScene.reset(newScene);
}
