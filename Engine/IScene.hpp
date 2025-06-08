#ifndef ISCENE_HPP
#define ISCENE_HPP

#include <allegro5/allegro.h>
#include "Group.hpp"

namespace Engine {
    class IScene : public Group {
    protected:
        IScene() = default;

    public:
        IScene(const IScene&) = delete;
        IScene& operator=(const IScene&) = delete;
        virtual ~IScene() = default;

        // 初始化與清理
        virtual void Initialize() = 0;
        virtual void Terminate();

        // 每幀邏輯與繪製
        virtual void Update(float deltaTime) override;
        virtual void Draw() const override;

        // 處理事件（鍵盤、滑鼠、關閉等等）
        virtual void OnEvent(ALLEGRO_EVENT& event) {};
    };
}

#endif // ISCENE_HPP
