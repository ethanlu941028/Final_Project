#include "IScene.hpp"

namespace Engine {

    void IScene::Terminate() {
        // 預設為清除場景中所有物件
        Clear();
    }

    void IScene::Update(float deltaTime) {
        // 更新場景中所有物件的狀態
        Group::Update(deltaTime);
    }

    void IScene::Draw() const {
        // 繪製場景中的所有物件
        Group::Draw();
    }

}
