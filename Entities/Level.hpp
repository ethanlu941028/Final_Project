#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <string>
#include <vector>

namespace Engine {
    class Group;
    class Point;
}
class Tile;

class Level {
public:
    Level(int width, int height, int tileSize, Engine::Group* group);
    bool LoadMap(const std::string& path);
    void InitializeView();
    void Scroll(float deltaTime, float speed);
    bool IsFinished() const;

private:
    int width;
    int height;
    int tileSize;
    float offsetX;
    bool finished;
    Engine::Group* container;
    std::vector<std::vector<Tile*>> tiles;
};

#endif // LEVEL_HPP