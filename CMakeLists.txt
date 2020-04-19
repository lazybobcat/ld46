cmake_minimum_required(VERSION 3.8.2)
project(ld46 VERSION 0.1.0)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/build)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_LIBS pthread sfml-system sfml-window sfml-graphics sfml-audio)
set(INTERNAL_LIBS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/potato)


# Lua
add_subdirectory(lua52)

# Game

set(SOURCE_FILES
        potato/controllers/playercontroller.cpp
        potato/entities/emitternode.cpp
        potato/entities/entity.cpp
        potato/entities/particlenode.cpp
        potato/entities/quadtree.cpp
        potato/entities/scenenode.cpp
        potato/entities/soundnode.cpp
        potato/entities/spritenode.cpp
        potato/entities/textnode.cpp
        potato/events/command.cpp
        potato/events/commandqueue.cpp
        potato/GUI/button.cpp
        potato/GUI/container.cpp
        potato/GUI/label.cpp
        potato/GUI/lineedit.cpp
        potato/GUI/widget.cpp
        potato/resources/affectors.cpp
        potato/resources/animation.cpp
        potato/resources/musicplayer.cpp
        potato/resources/particle.cpp
        potato/resources/scriptplayer.cpp
        potato/resources/soundplayer.cpp
        potato/states/paralleltask.cpp
        potato/states/state.cpp
        potato/states/statestack.cpp
        potato/states/titlestate.cpp
        potato/application.cpp
        potato/datatables.cpp
        potato/utils.cpp
        potato/utils.inl
        potato/world.cpp
        game_state.hpp game_state.cpp
        main.cpp pause_state.cpp pause_state.hpp bloom_effect.cpp bloom_effect.hpp potato/resources/post_effect.cpp potato/resources/post_effect.hpp field_node.cpp field_node.hpp player_entity.cpp player_entity.hpp well_node.cpp well_node.hpp points_node.cpp points_node.hpp)

add_executable(ld46 ${SOURCE_FILES})
target_link_libraries(ld46 ${INTERNAL_LIBS} ${EXTERNAL_LIBS} lua)
