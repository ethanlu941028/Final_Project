#include <allegro5/allegro.h>
#include "Engine/GameEngine.hpp"
#include "States/TitleScreen.hpp"
#include "States/Gameplay.hpp"  // 如果有的話

int main(int argc, char **argv) {
	// 加入你自己想要的場景
	Engine::GameEngine& engine = Engine::GameEngine::GetInstance();

	// 新增場景
	engine.AddNewScene("title", new TitleScreen());
	//engine.AddNewScene("play", new Gameplay()); // 如果你還沒實作可以先留空或用暫時的 scene

	// 啟動遊戲，從 title scene 開始
	engine.Start("title", 60, 1600, 832);

	return 0;
}
