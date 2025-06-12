cmake_minimum_required(VERSION 3.27)
project(2025_I2P2_TowerDefense)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(Resource)

file(COPY Resource/audios DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(2025_I2P2_TowerDefense

    # TODO HACKATHON-3 (3/3): Add the new enemy source files here

    Engine/Allegro5Exception.cpp
    Engine/Allegro5Exception.hpp
    Engine/AudioHelper.cpp
    Engine/AudioHelper.hpp
    Engine/Collider.cpp
    Engine/Collider.hpp
    Engine/GameEngine.cpp
    Engine/GameEngine.hpp
    Engine/Group.cpp
    Engine/Group.hpp
    Engine/IControl.cpp
    Engine/IControl.hpp
    Engine/IObject.cpp
    Engine/IObject.hpp
    Engine/IScene.cpp
    Engine/IScene.hpp
    Engine/LOG.cpp
    Engine/LOG.hpp
    Engine/Point.cpp
    Engine/Point.hpp
    Engine/Resources.cpp
    Engine/Resources.hpp
    Engine/Sprite.cpp
    Engine/Sprite.hpp

    Entities/Player.cpp
    Entities/Player.hpp
    Entities/Title.cpp
    Entities/Title.hpp
    Entities/Level.cpp
    Entities/Level.hpp
    Entities/Tile.cpp
    Entities/Tile.hpp
    Entities/AirTile.cpp
    Entities/AirTile.hpp
    Entities/GroundTile.cpp
    Entities/GroundTile.hpp
    Entities/SpikeTile.cpp
    Entities/SpikeTile.hpp
    # TODO HACKATHON-2 (3/3): Add the new scene source files here

    States/DeathScreen.cpp
    States/DeathScreen.hpp
    States/Gameplay.cpp
    States/Gameplay.hpp
    States/TitleScreen.cpp
    States/TitleScreen.hpp
    States/PauseMenu.cpp
    States/PauseMenu.hpp
    States/StageSelect.cpp
    States/StageSelect.hpp
    States/SettingScreen.cpp
    States/SettingScreen.hpp
    States/WinScreen.cpp
    States/WinScreen.hpp

    UI/Component/Image.cpp
    UI/Component/Image.hpp
    UI/Component/ImageButton.cpp
    UI/Component/ImageButton.hpp
    UI/Component/Label.cpp
    UI/Component/Label.hpp
    UI/Component/Slider.cpp
    UI/Component/Slider.hpp

    Utils/Config.cpp
    Utils/Config.hpp
    Utils/Timer.cpp
    Utils/Timer.hpp
    Utils/Config.cpp
    Utils/Resources.cpp
    Utils/Resources.hpp
    
    main.cpp
        States/TitleScreen.hpp
        States/TitleScreen.cpp
        States/Gameplay.cpp
        States/Gameplay.hpp
        States/DeathScreen.hpp
        States/DeathScreen.cpp
        Entities/Player.cpp
        Entities/Player.hpp
        Entities/Title.cpp
        Entities/Title.hpp
        Entities/Level.cpp
        Entities/Level.hpp
        Entities/Tile.cpp
        Entities/Tile.hpp
        Entities/AirTile.cpp
        Entities/AirTile.hpp
        Entities/GroundTile.cpp
        Entities/GroundTile.hpp
        Entities/SpikeTile.cpp
        Entities/SpikeTile.hpp
        Utils/Timer.cpp
        Utils/Timer.hpp
        Utils/Config.cpp
        Utils/Config.hpp
        States/PauseMenu.cpp
        States/PauseMenu.hpp
        Utils/Resources.cpp
        Utils/Resources.hpp
        Game.cpp
        Game.hpp
        States/StageSelect.cpp
        States/StageSelect.hpp
        States/SettingScreen.cpp
        States/SettingScreen.hpp
        States/WinScreen.cpp
        States/WinScreen.hpp
        States/ScoreboardScene.cpp
        States/ScoreboardScene.hpp
)

IF (WIN32)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC C:/allegro/include)
    target_link_directories(2025_I2P2_TowerDefense PUBLIC C:/allegro/lib)
    target_compile_options(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_TowerDefense liballegro_monolith)
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC lib)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
    target_compile_options(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_TowerDefense ${ALLEGRO5_LIBRARIES})
    target_link_directories(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_LIBRARY_DIRS})
ENDIF()