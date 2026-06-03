#include "Target.h"
#include "Utils.h"

Target::Target(int id) : m_id(id), m_type(TargetType::UNKNOWN), m_status(TargetStatus::NEUTRAL), 
                         m_x(0), m_y(0), m_alt(0), m_speed(0), m_heading(0), m_range(0), m_bearing(0) {
    m_callsign = "TGT-" + std::to_string(id);
}

void Target::update(double dt) {
    if (!m_active) return;
    updatePos(dt);
    calcPolar();
    
    m_historyTimer += dt;
    if (m_historyTimer > 1.0) {
        m_historyTimer = 0;
        m_history.push_back({m_x, m_y});
        if (m_history.size() > 50) m_history.pop_front();
    }
}

void Target::updatePos(double dt) {
    double rad = Utils::degToRad(m_heading);
    m_x += m_speed * std::sin(rad) * (dt / 3600.0);
    m_y -= m_speed * std::cos(rad) * (dt / 3600.0);
}

void Target::setPosition(double x, double y) { 
    m_x = x; 
    m_y = y; 
    calcPolar(); 
}

void Target::calcPolar() {
    Utils::cartesianToPolar(m_x, m_y, m_range, m_bearing);
}

sf::Color Target::getColor() const {
    if (m_status == TargetStatus::HOSTILE) return sf::Color(255, 50, 50); // Red
    if (m_status == TargetStatus::FRIENDLY) return sf::Color(0, 255, 100); // Green
    return sf::Color(255, 255, 0); // Yellow for Neutral/Unknown
}

// --- REALISTIC FACTORY METHODS ---

Target Target::createAirliner(int id, double maxRange) {
    Target t(id);
    double range = Utils::randomDouble(100, maxRange * 0.8);
    double bearing = Utils::randomDouble(0, 360);
    double x, y;
    Utils::polarToCartesian(range, bearing, x, y);
    
    t.setPosition(x, y);
    t.m_speed = Utils::randomDouble(800, 950); // Cruise speed ~900km/h
    t.m_heading = Utils::randomDouble(0, 360);
    t.m_alt = Utils::randomDouble(9, 12); // Flight Level 300-400
    t.m_type = TargetType::AIRLINER;
    t.m_status = TargetStatus::FRIENDLY;
    
    // Realistic Callsigns
    std::string airlines[] = {"UAL", "DAL", "BAW", "AAL", "UAE"};
    t.m_callsign = airlines[Utils::randomInt(0, 4)] + std::to_string(Utils::randomInt(100, 999));
    return t;
}

Target Target::createFighter(int id, double maxRange) {
    Target t(id);
    double range = Utils::randomDouble(50, maxRange * 0.6);
    double bearing = Utils::randomDouble(0, 360);
    double x, y;
    Utils::polarToCartesian(range, bearing, x, y);
    
    t.setPosition(x, y);
    t.m_speed = Utils::randomDouble(1200, 2500); // Supersonic
    t.m_heading = Utils::randomDouble(0, 360);
    t.m_alt = Utils::randomDouble(5, 15);
    t.m_type = TargetType::FIGHTER;
    t.m_status = (Utils::randomInt(0, 3) == 0) ? TargetStatus::HOSTILE : TargetStatus::FRIENDLY;
    t.m_callsign = "VIPER" + std::to_string(Utils::randomInt(10, 99));
    return t;
}

Target Target::createMissile(int id, double maxRange) {
    Target t(id);
    // Spawn at edge flying IN
    double bearing = Utils::randomDouble(0, 360);
    double x, y;
    Utils::polarToCartesian(maxRange * 0.9, bearing, x, y);
    
    t.setPosition(x, y);
    t.m_speed = Utils::randomDouble(3000, 6000); // Very fast
    t.m_heading = fmod(bearing + 180, 360); // Fly towards center
    t.m_alt = Utils::randomDouble(1, 5);
    t.m_type = TargetType::MISSILE;
    t.m_status = TargetStatus::HOSTILE;
    t.m_callsign = "INCOMING";
    return t;
}

Target Target::createShip(int id, double maxRange) {
    Target t(id);
    double range = Utils::randomDouble(20, maxRange * 0.4); // Ships are closer usually
    double bearing = Utils::randomDouble(0, 360);
    double x, y;
    Utils::polarToCartesian(range, bearing, x, y);
    
    t.setPosition(x, y);
    t.m_speed = Utils::randomDouble(30, 60); // Slow
    t.m_heading = Utils::randomDouble(0, 360);
    t.m_alt = 0; // Sea level
    t.m_type = TargetType::CRUISER;
    t.m_status = TargetStatus::NEUTRAL;
    t.m_callsign = "SHIP" + std::to_string(Utils::randomInt(10, 99));
    return t;
}