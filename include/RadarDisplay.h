#ifndef RADAR_DISPLAY_H
#define RADAR_DISPLAY_H

#include <SFML/Graphics.hpp>
#include "RadarSystem.h"

class RadarDisplay {
public:
    RadarDisplay(sf::RenderWindow& win, RadarSystem& radar);
    
    bool init();
    void render();
    void handleClick(sf::Vector2f pos);
    void handleScroll(float delta);
    
    int getSelectedId() const { return m_selectedId; }
    double getMaxRangeKm() const { return m_viewRange; } // Accessor for GUI

private:
    void drawGrid();
    void drawSweep();
    void drawTargets();
    void drawUI();
    sf::Vector2f toScreen(double x, double y) const;
    
    sf::RenderWindow& m_win;
    RadarSystem& m_radar;
    sf::Font m_font;
    bool m_fontLoaded = false;
    
    sf::Vector2f m_center;
    float m_radius;
    double m_viewRange = 500.0; 
    int m_selectedId = -1;
};

#endif