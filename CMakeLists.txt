cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ld54 CXX)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(ld54 main.cpp
    sources/utils/vector2d.cpp
    sources/animation.cpp
    sources/audio_source.cpp
    sources/button.cpp
    sources/game_object.cpp
    sources/particle.cpp
    sources/player.cpp
    sources/physics_object.cpp
    sources/text.cpp
    sources/texture.cpp
    sources/tile.cpp
    sources/tile_map.cpp
    sources/timer.cpp
    sources/ui_object.cpp
    sources/ui_state_object.cpp
    sources/input_field.cpp
    sources/canvas.cpp
    sources/image.cpp
    sources/ui_panel.cpp
    sources/scene.cpp
    sources/game/game.cpp
    sources/game/box.cpp
    sources/game/cursor.cpp
    sources/game/furniture.cpp)

target_link_libraries(ld54 ${SDL2_LIBRARIES})
target_link_libraries(ld54 SDL2_ttf::SDL2_ttf)
target_link_libraries(ld54 SDL2_image::SDL2_image)
target_link_libraries(ld54 SDL2_mixer::SDL2_mixer)

add_custom_command(TARGET ld54 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:ld54>/resources/)