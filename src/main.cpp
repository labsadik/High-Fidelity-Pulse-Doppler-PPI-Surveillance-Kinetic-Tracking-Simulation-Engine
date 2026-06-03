#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include "RadarSystem.h"
#include "RadarDisplay.h"
#include "GUI.h"

int main() {
    // 1. Create Window
    sf::RenderWindow window(sf::VideoMode({1400, 900}), "Radar Simulator 10000km");
    window.setFramerateLimit(60);

    // 2. Initialize Systems
    RadarSystem radar;
    radar.init();
    
    RadarDisplay display(window, radar);
    if (!display.init()) {
        std::cerr << "Failed to init display\n";
        return -1;
    }
    
    GUI gui(window, radar, display);
    gui.init(); // Initialize GUI (safe init)

    // 3. Timing
    auto lastTime = std::chrono::high_resolution_clock::now();

    // 4. Main Loop
    while (window.isOpen()) {
        // Calculate Delta Time
        auto now = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double>(now - lastTime).count();
        lastTime = now;

        // Handle Events
        bool clicked = false;
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* mp = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mp->button == sf::Mouse::Button::Left) {
                    clicked = true;
                }
            }
            else if (const auto* mw = event->getIf<sf::Event::MouseWheelScrolled>()) {
                display.handleScroll(mw->delta);
            }
        }

        // Input Handling
        sf::Vector2f mousePos(sf::Vector2f(sf::Mouse::getPosition(window)));
        
        // Pass click to GUI first, if not consumed, pass to Display (Radar)
        if (clicked) {
            gui.handleEvent(mousePos, true);
            // Only handle display click if not clicking on GUI (simple check: mouse X < window width - 200)
            if (mousePos.x < window.getSize().x - 200) {
                display.handleClick(mousePos);
            }
        }

        // Update Logic
        radar.update(dt);

        // Render
        window.clear(sf::Color(0, 5, 0));
        
        display.render();
        gui.render();

        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}