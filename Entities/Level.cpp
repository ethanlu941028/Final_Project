#include "Level.hpp"
#include "AirTile.hpp"
#include "GroundTile.hpp"
#include "SpikeTile.hpp"
#include "FlipOrb.hpp"
#include "JumpOrb.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include <fstream>
#include <algorithm>

Level::Level(int w, int h, Engine::Group* group)
    : width(w), height(h), offsetX(0), finished(false), spawnedColumns(0), container(group) {}

bool Level::LoadMap(const std::string& path) {
    return LoadFromFile(path);
}
bool Level::LoadFromFile(const std::string& path) {
    std::ifstream fin(path);
    if (!fin.is_open()) return false;
    tiles.assign(height, std::vector<Tile*>(width, nullptr));
    std::string line;
    int y = 0;
    while (y < height && std::getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        for (int x = 0; x < width; ++x) {
            char c = (x < static_cast<int>(line.size())) ? line[x] : '0';
            if (c == '1') tiles[y][x] = new GroundTile(x, y);
            else if (c == '2') tiles[y][x] = new SpikeTile(x, y);
            else if (c == '3') tiles[y][x] = new FlipOrb(x, y);
            else if (c == '4') tiles[y][x] = new JumpOrb(x, y);
            else tiles[y][x] = new AirTile(x, y);
        }
        ++y;
    }
    // Fill remaining rows with empty tiles if file has fewer lines
    for (; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            tiles[y][x] = new AirTile(x, y);
        }
    }
    return true;
}

void Level::InitializeView() {
    int screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
    spawnedColumns = std::min(width, screenW / TILE_SIZE + 1);
    if (!container) return;
    for (auto& row : tiles) {
        for (int x = 0; x < spawnedColumns; ++x) {
            Tile* tile = row[x];
            if (tile) container->AddNewObject(tile);
        }
    }
}

void Level::Scroll(float deltaTime, float speed) {
    if (finished) return;
    offsetX += speed * deltaTime;
    int screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
    float maxOffset = width * TILE_SIZE - screenW;
    // Spawn new columns when they enter the visible area
    int lastVisibleCol = static_cast<int>(offsetX / TILE_SIZE) + screenW / TILE_SIZE + 1;
    while (spawnedColumns < width && spawnedColumns <= lastVisibleCol) {
        for (int y = 0; y < height; ++y) {
            Tile* tile = tiles[y][spawnedColumns];
            if (tile) container->AddNewObject(tile);
        }
        ++spawnedColumns;
    }

    if (spawnedColumns >= width && offsetX >= maxOffset) {
        offsetX = maxOffset;
        finished = true;
    }
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < spawnedColumns; ++x) {
            Tile* tile = tiles[y][x];
            if (tile) tile->Position.x = x * TILE_SIZE - offsetX;
        }
    }
}

bool Level::IsFinished() const {
    return finished;
}