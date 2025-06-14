//
// Created by 盧昱辰 on 2025/6/8.
//

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>

// Global tile size used throughout the game.
constexpr std::int32_t TILE_SIZE = 64;

// Runtime flag controlling whether hitboxes are drawn.
extern bool showHitbox;

// Persistent settings toggled from the settings menus.
extern bool showHitboxSetting;
extern bool cheatModeSetting;

#endif //CONFIG_HPP
