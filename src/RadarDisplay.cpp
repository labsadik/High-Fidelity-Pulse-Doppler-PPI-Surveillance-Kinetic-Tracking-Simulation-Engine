#include "RadarDisplay.h"
#include "Utils.h"
#include <iostream>
#include <cmath>

RadarDisplay::RadarDisplay(sf::RenderWindow& win, RadarSystem& radar) 
    : m_win(win), m_radar(radar) {
    
    auto size = m_win.getSize();
    m_center = { size.x / 2.0f, size.y / 2.0f };
    m_radius = std::min(size.x, size.y) / 2.0f - 80.0f;
}

bool RadarDisplay::init() {
    if (m_font.openFromFile("C:/Windows/Fonts/consola.ttf")) m_fontLoaded = true;
    else if (m_font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf")) m_fontLoaded = true;
    return true;
}

void RadarDisplay::render() {
    sf::CircleShape bg(m_radius);
    bg.setFillColor(sf::Color(0, 15, 0));
    bg.setPosition({ m_center.x - m_radius, m_center.y - m_radius });
    m_win.draw(bg);

    drawGrid();
    drawSweep();
    drawTargets();
    drawUI();
}

void RadarDisplay::drawGrid() {
    for (int i = 1; i <= 5; ++i) {
        float r = (i / 5.0f) * m_radius;
        sf::CircleShape ring;
        ring.setRadius(r);
        ring.setPosition({ m_center.x - r, m_center.y - r });
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineColor(sf::Color(0, 100, 0));
        ring.setOutlineThickness(1);
        m_win.draw(ring);
        
        if (m_fontLoaded) {
            sf::Text txt(m_font);
            txt.setString(std::to_string(static_cast<int>(m_viewRange * i / 5)) + " km");
            txt.setCharacterSize(10);
            txt.setFillColor(sf::Color(0, 150, 0));
            txt.setPosition({ m_center.x + 5, m_center.y - r - 10 });
            m_win.draw(txt);
        }
    }
    
    for (int b = 0; b < 360; b += 30) {
        double rad = Utils::degToRad(b);
        float x2 = static_cast<float>(m_center.x + m_radius * std::sin(rad));
        float y2 = static_cast<float>(m_center.y - m_radius * std::cos(rad));
        sf::Vertex line[] = { { m_center, sf::Color(0, 80, 0) }, { { x2, y2 }, sf::Color(0, 80, 0) } };
        m_win.draw(line, 2, sf::PrimitiveType::LineStrip);
    }
}

void RadarDisplay::drawSweep() {
    double rad = Utils::degToRad(m_radar.getSweepAngle());
    float x2 = static_cast<float>(m_center.x + m_radius * std::sin(rad));
    float y2 = static_cast<float>(m_center.y - m_radius * std::cos(rad));
    sf::Vertex line[] = { { m_center, sf::Color::Green }, { { x2, y2 }, sf::Color::Green } };
    m_win.draw(line, 2, sf::PrimitiveType::LineStrip);
    
    sf::VertexArray glow(sf::PrimitiveType::TriangleFan);
    glow.append({ m_center, sf::Color(0, 50, 0, 100) });
    for (int i = 0; i <= 10; ++i) {
        double a = rad - Utils::degToRad(15.0 * i / 10.0);
        float gx = static_cast<float>(m_center.x + m_radius * std::sin(a));
        float gy = static_cast<float>(m_center.y - m_radius * std::cos(a));
        glow.append({ { gx, gy }, sf::Color(0, 20, 0, 0) });
    }
    m_win.draw(glow);
}

void RadarDisplay::drawTargets() {
    const auto& targets = m_radar.getTargets();
    
    for (const auto& t : targets) {
        if (!t.isDetected() || !t.isActive()) continue;
        
        sf::Vector2f pos = toScreen(t.getX(), t.getY());
        
        // Draw trail
        const auto& hist = t.getHistory();
        if (hist.size() > 1) {
            sf::VertexArray trail(sf::PrimitiveType::LineStrip);
            for (const auto& p : hist) trail.append({ toScreen(p.x, p.y), sf::Color(0, 255, 0, 50) });
            m_win.draw(trail);
        }
        
        // Draw specific shapes based on type
        TargetType type = t.getType();
        sf::Color col = t.getColor();

        if (type == TargetType::AIRLINER) {
            // Large Triangle
            sf::ConvexShape tri(3);
            tri.setPoint(0, {0, -8});
            tri.setPoint(1, {-6, 6});
            tri.setPoint(2, {6, 6});
            tri.setPosition(pos);
            tri.setRotation(sf::degrees(t.getHeading()));
            tri.setFillColor(col);
            m_win.draw(tri);
        } 
        else if (type == TargetType::FIGHTER) {
            // Small fast triangle
            sf::ConvexShape tri(3);
            tri.setPoint(0, {0, -6});
            tri.setPoint(1, {-4, 4});
            tri.setPoint(2, {4, 4});
            tri.setPosition(pos);
            tri.setRotation(sf::degrees(t.getHeading()));
            tri.setFillColor(col);
            m_win.draw(tri);
        }
        else if (type == TargetType::MISSILE) {
            // Tiny dot with high intensity
            sf::CircleShape dot(3);
            dot.setPosition({pos.x - 3, pos.y - 3});
            dot.setFillColor(sf::Color::Red); // Always red
            m_win.draw(dot);
        }
        else if (type == TargetType::CRUISER || type == TargetType::CARRIER) {
            // Square for ships
            sf::RectangleShape rect({8, 8});
            rect.setPosition({pos.x - 4, pos.y - 4});
            rect.setFillColor(col);
            m_win.draw(rect);
        }
        else {
            // Default Circle
            sf::CircleShape blip(5);
            blip.setPosition({pos.x - 5, pos.y - 5});
            blip.setFillColor(col);
            m_win.draw(blip);
        }
        
        // Labels
        if (m_fontLoaded) {
            sf::Text label(m_font);
            label.setString(t.getCallsign());
            label.setCharacterSize(10);
            label.setFillColor(col);
            label.setPosition({pos.x + 8, pos.y - 8});
            m_win.draw(label);
            
            // Altitude label for aircraft
            if (type == TargetType::AIRLINER || type == TargetType::FIGHTER) {
                sf::Text altLbl(m_font);
                altLbl.setCharacterSize(8);
                altLbl.setFillColor(sf::Color(150, 255, 150));
                // Convert km to flight level (approx)
                int fl = static_cast<int>(t.getAlt() * 32.8);
                altLbl.setString("FL" + std::to_string(fl));
                altLbl.setPosition({pos.x + 8, pos.y + 2});
                m_win.draw(altLbl);
            }
        }
        
        // Selection highlight
        if (t.getId() == m_selectedId) {
            sf::CircleShape sel(15);
            sel.setPosition({pos.x - 15, pos.y - 15});
            sel.setFillColor(sf::Color::Transparent);
            sel.setOutlineColor(sf::Color::White);
            sel.setOutlineThickness(2);
            m_win.draw(sel);
        }
    }
}

void RadarDisplay::drawUI() {
    if (!m_fontLoaded) return;
    sf::Text info(m_font);
    info.setString("RADAR OPS CENTER\nZoom: " + std::to_string(static_cast<int>(m_viewRange)) + " km");
    info.setFillColor(sf::Color::Green);
    info.setPosition({ 10, 10 });
    m_win.draw(info);
    
    if (m_selectedId >= 0) {
        Target* t = m_radar.getTarget(m_selectedId);
        if (t) {
            sf::Text tgtInfo(m_font);
            tgtInfo.setPosition({ 10, 60 });
            tgtInfo.setFillColor(t->getColor());
            
            std::string typeStr = "Unknown";
            if(t->getType() == TargetType::AIRLINER) typeStr = "Airliner";
            if(t->getType() == TargetType::FIGHTER) typeStr = "Fighter";
            if(t->getType() == TargetType::MISSILE) typeStr = "MISSILE";
            if(t->getType() == TargetType::CRUISER) typeStr = "Ship";

            tgtInfo.setString(
                "TARGET: " + t->getCallsign() + "\n" +
                "TYPE: " + typeStr + "\n" +
                "RANGE: " + std::to_string(static_cast<int>(t->getRange())) + " km\n" +
                "SPD: " + std::to_string(static_cast<int>(t->getSpeed())) + " km/h"
            );
            m_win.draw(tgtInfo);
        }
    }
}

sf::Vector2f RadarDisplay::toScreen(double x, double y) const {
    float sx = m_center.x + static_cast<float>((x / m_viewRange) * m_radius);
    float sy = m_center.y - static_cast<float>((y / m_viewRange) * m_radius);
    return { sx, sy };
}

void RadarDisplay::handleClick(sf::Vector2f pos) {
    m_selectedId = -1;
    double dist = std::sqrt(std::pow(pos.x - m_center.x, 2) + std::pow(pos.y - m_center.y, 2));
    if (dist > m_radius) return;

    for (const auto& t : m_radar.getTargets()) {
        if (!t.isDetected()) continue;
        sf::Vector2f tpos = toScreen(t.getX(), t.getY());
        double d = std::sqrt(std::pow(pos.x - tpos.x, 2) + std::pow(pos.y - tpos.y, 2));
        if (d < 20) {
            m_selectedId = t.getId();
            return;
        }
    }
}

void RadarDisplay::handleScroll(float delta) {
    if (delta > 0) m_viewRange *= 0.8;
    else m_viewRange *= 1.25;
    
    if (m_viewRange < 10) m_viewRange = 10;
    if (m_viewRange > 10000) m_viewRange = 10000;
}