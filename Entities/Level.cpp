//
// Created by 盧昱辰 on 2025/6/8.
//
#include "Level.hpp"

#include <fstream>
#include <stdexcept>

static std::unique_ptr<Tile> CreateTile(char c, int x, int y, int tileSize) {
    if (c == '1')
        return std::make_unique<GroundTile>(x, y, tileSize);
    return std::make_unique<AirTile>(x, y, tileSize);
}

void Level::LoadFromFile(const std::string& path) {
    rows.clear();
    std::ifstream fin(path);
    if (!fin.is_open())
        throw std::runtime_error("Failed to open level file: " + path);
    std::string line;
    while (std::getline(fin, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        rows.push_back(line);
    }
    fin.close();
}

void Level::InitializeView(int vColumns, int vRows) {
    visibleColumns = vColumns;
    visibleRows = vRows;
    startColumn = 0;
    scrollOffset = 0.0f;
    activeTiles.clear();
    for (int y = 0; y < visibleRows && y < static_cast<int>(rows.size()); ++y) {
        for (int x = 0; x < visibleColumns && x < static_cast<int>(rows[y].size()); ++x) {
            activeTiles.emplace_back(CreateTile(rows[y][x], x, y, tileSize));
        }
    }
}

void Level::Scroll(float deltaTime, float speed) {
    if (IsFinished())
        return;

    scrollOffset += speed * deltaTime;
    int shift = static_cast<int>(scrollOffset / tileSize);
    if (shift > 0) {
        scrollOffset -= shift * tileSize;
        startColumn += shift;
        activeTiles.clear();
        for (int y = 0; y < visibleRows && y < static_cast<int>(rows.size()); ++y) {
            for (int x = 0; x < visibleColumns && startColumn + x < static_cast<int>(rows[y].size()); ++x) {
                activeTiles.emplace_back(CreateTile(rows[y][startColumn + x], x, y, tileSize));
            }
        }
    }
    for (size_t i = 0; i < activeTiles.size(); ++i) {
        int col = i % visibleColumns;
        activeTiles[i]->Position.x = col * tileSize - scrollOffset;
    }
}

bool Level::IsFinished() const {
    int width = rows.empty() ? 0 : static_cast<int>(rows[0].size());
    return startColumn + visibleColumns >= width;
}
