cmake_minimum_required(VERSION 3.16)
project(AdvancedRadarSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML 3
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

add_executable(radar_sim
    src/main.cpp
    src/Utils.cpp
    src/Target.cpp
    src/RadarSystem.cpp
    src/RadarDisplay.cpp
    src/GUI.cpp
)

target_include_directories(radar_sim PRIVATE include)

# Link SFML 3 libraries
target_link_libraries(radar_sim 
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)