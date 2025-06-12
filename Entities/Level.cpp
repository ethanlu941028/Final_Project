#include "Level.hpp"
#include "AirTile.hpp"
#include "GroundTile.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include <fstream>

Level::Level(int w, int h, Engine::Group* group)
    : width(w), height(h), offsetX(0), finished(false), container(group) {}

bool Level::LoadMap(const std::string& path) {
    std::ifstream fin(path);
    if (!fin.is_open()) return false;
    tiles.assign(height, std::vector<Tile*>(width, nullptr));
    std::string line;
    for (int y = 0; y < height; ++y) {
        if (!std::getline(fin, line)) return false;
        for (int x = 0; x < width && x < static_cast<int>(line.size()); ++x) {
            char c = line[x];
            if (c == '1')
                tiles[y][x] = new GroundTile(x, y);
            else
            tiles[y][x] = new AirTile(x, y);
        }
    }
    return true;
}

void Level::InitializeView() {
    if (!container) return;
    for (auto& row : tiles) {
        for (auto* tile : row) {
            if (tile) container->AddNewObject(tile);
        }
    }
}

void Level::Scroll(float deltaTime, float speed) {
    if (finished) return;
    offsetX += speed * deltaTime;
    int screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
    float maxOffset = width * TILE_SIZE - screenW;
    if (offsetX >= maxOffset) {
        offsetX = maxOffset;
        finished = true;
    }
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Tile* tile = tiles[y][x];
            if (tile) tile->Position.x = x * TILE_SIZE - offsetX;
        }
    }
}

bool Level::IsFinished() const {
    return finished;
}