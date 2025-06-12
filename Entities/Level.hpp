//
// Created by 盧昱辰 on 2025/6/8.
//
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <memory>
#include <string>
#include <vector>

#include "AirTile.hpp"
#include "GroundTile.hpp"

class Level {
public:
    // Store raw text map loaded from file.
    std::vector<std::string> rows;

private:
    std::vector<std::unique_ptr<Tile>> activeTiles;
    int visibleColumns = 0;
    int visibleRows = 0;
    int startColumn = 0;
    float scrollOffset = 0.0f;
    const int tileSize = 64;

public:
    Level() = default;

    void LoadFromFile(const std::string& path);
    void InitializeView(int visibleColumns, int visibleRows);
    void Scroll(float deltaTime, float speed);
    bool IsFinished() const;

    const std::vector<std::unique_ptr<Tile>>& GetActiveTiles() const { return activeTiles; }
    int GetStartColumn() const { return startColumn; }
};

#endif // LEVEL_HPP
