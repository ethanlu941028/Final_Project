#include <allegro5/allegro.h>
#include "Engine/GameEngine.hpp"
#include "States/TitleScreen.hpp"
#include "States/Gameplay.hpp"
#include "States/DeathScreen.hpp"
#include "States/PauseMenu.hpp"
#include "States/StageSelect.hpp"
#include "States/SettingScreen.hpp"
#include <iostream>


int main(int argc, char **argv) {
	// 加入你自己想要的場景
	Engine::GameEngine& engine = Engine::GameEngine::GetInstance();
	printf("Welcome to the game!\n");

	// 新增場景
	engine.AddNewScene("title", new TitleScreen());
	engine.AddNewScene("play", new Gameplay());
	engine.AddNewScene("pause", new PauseMenu());
	engine.AddNewScene("death", new DeathScreen());
	engine.AddNewScene("stage-select", new StageSelect());
	engine.AddNewScene("setting", new SettingScreen());
	// 啟動遊戲，從 title scene 開始
	engine.Start("title", 60, 1600, 832);

	return 0;
}
