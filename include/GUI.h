#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <functional>
#include "RadarSystem.h"
#include "RadarDisplay.h"

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> callback;

    Button(sf::Vector2f pos, sf::Vector2f size, const std::string& label, const sf::Font& font);
    void draw(sf::RenderTarget& win);
    bool checkClick(sf::Vector2f mousePos);
};

class GUI {
public:
    GUI(sf::RenderWindow& win, RadarSystem& radar, RadarDisplay& display);
    bool init();
    void handleEvent(sf::Vector2f mousePos, bool clicked);
    void render();

private:
    sf::RenderWindow& m_win;
    RadarSystem& m_radar;
    RadarDisplay& m_display;
    sf::Font m_font;
    bool m_fontLoaded = false;
    
    std::vector<Button> m_buttons;
    sf::RectangleShape m_panelBg;
};

#endif