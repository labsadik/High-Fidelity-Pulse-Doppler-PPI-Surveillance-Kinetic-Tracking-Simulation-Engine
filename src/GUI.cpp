#include "GUI.h"
#include "Utils.h" // FIXED: Added missing include
#include <iostream>

Button::Button(sf::Vector2f pos, sf::Vector2f size, const std::string& label, const sf::Font& font) 
    : text(font) {
    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(sf::Color(40, 40, 40));
    shape.setOutlineColor(sf::Color(0, 255, 0));
    shape.setOutlineThickness(1);
    
    text.setString(label);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Green);
    text.setPosition({ pos.x + 10, pos.y + 5 });
}

void Button::draw(sf::RenderTarget& win) {
    win.draw(shape);
    win.draw(text);
}

bool Button::checkClick(sf::Vector2f mp) {
    if (shape.getGlobalBounds().contains(mp)) {
        if (callback) callback();
        return true;
    }
    return false;
}

GUI::GUI(sf::RenderWindow& win, RadarSystem& radar, RadarDisplay& display) 
    : m_win(win), m_radar(radar), m_display(display) {
}

bool GUI::init() {
    if (m_font.openFromFile("C:/Windows/Fonts/consola.ttf")) m_fontLoaded = true;
    else if (m_font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf")) m_fontLoaded = true;
    
    float panelWidth = 200.0f;
    float winWidth = static_cast<float>(m_win.getSize().x);
    float winHeight = static_cast<float>(m_win.getSize().y);
    
    m_panelBg.setSize({panelWidth, winHeight});
    m_panelBg.setPosition({winWidth - panelWidth, 0});
    m_panelBg.setFillColor(sf::Color(10, 20, 10, 240));

    if (m_fontLoaded) {
        float startX = winWidth - panelWidth + 20;
        float startY = 20;

        // Row 1
        m_buttons.push_back(Button({startX, startY}, {160, 30}, "Add Airliner", m_font));
        m_buttons.back().callback = [this](){ m_radar.spawnAirliner(); };
        
        // Row 2
        m_buttons.push_back(Button({startX, startY + 40}, {160, 30}, "Add Fighter", m_font));
        m_buttons.back().callback = [this](){ m_radar.spawnFighter(); };

        // Row 3
        m_buttons.push_back(Button({startX, startY + 80}, {160, 30}, "Add Missile", m_font));
        m_buttons.back().callback = [this](){ m_radar.spawnMissile(); };

        // Row 4
        m_buttons.push_back(Button({startX, startY + 120}, {160, 30}, "Add Ship", m_font));
        m_buttons.back().callback = [this](){ m_radar.spawnShip(); };

        // Row 5
        m_buttons.push_back(Button({startX, startY + 180}, {160, 30}, "Clear All", m_font));
        m_buttons.back().callback = [this](){ m_radar.clearTargets(); };
    }
    
    return true;
}

void GUI::handleEvent(sf::Vector2f mp, bool clicked) {
    if (clicked) {
        for (auto& btn : m_buttons) btn.checkClick(mp);
    }
}

void GUI::render() {
    m_win.draw(m_panelBg);
    for (auto& btn : m_buttons) btn.draw(m_win);
    
    if (m_fontLoaded) {
        sf::Text stats(m_font);
        stats.setCharacterSize(14);
        stats.setFillColor(sf::Color(100, 255, 100));
        stats.setPosition({ m_win.getSize().x - 180.0f, 300.0f });
        
        std::string str = "SYSTEM STATUS\n\n";
        str += "Time: " + Utils::formatTime(static_cast<int>(m_radar.getTime())) + "\n";
        str += "Targets: " + std::to_string(m_radar.getTargets().size()) + "\n";
        str += "Range: " + std::to_string(static_cast<int>(m_display.getMaxRangeKm())) + " km";
        
        stats.setString(str);
        m_win.draw(stats);
    }
}