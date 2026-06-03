#ifndef TARGET_H
#define TARGET_H

#include <string>
#include <deque>
#include <SFML/Graphics.hpp>

// Realistic Target Types
enum class TargetType { 
    AIRLINER,      // Commercial planes
    FIGHTER,       // Military jets
    MISSILE,       // Incoming threats
    CRUISER,       // Ships
    CARRIER,       // Aircraft Carrier
    UNKNOWN 
};

enum class TargetStatus { FRIENDLY, HOSTILE, NEUTRAL };

struct TrackPoint {
    double x, y;
};

class Target {
public:
    Target(int id);
    
    void update(double dt);
    
    int getId() const { return m_id; }
    const std::string& getCallsign() const { return m_callsign; }
    TargetType getType() const { return m_type; }
    TargetStatus getStatus() const { return m_status; }
    
    double getX() const { return m_x; }
    double getY() const { return m_y; }
    double getAlt() const { return m_alt; }
    double getSpeed() const { return m_speed; }
    double getHeading() const { return m_heading; }
    double getRange() const { return m_range; }
    double getBearing() const { return m_bearing; }
    
    bool isDetected() const { return m_detected; }
    bool isActive() const { return m_active; }
    
    void setPosition(double x, double y);
    void setDetected(bool d) { m_detected = d; }
    void setType(TargetType t) { m_type = t; }
    void setStatus(TargetStatus s) { m_status = s; }
    void setHeading(double h) { m_heading = h; }
    void setSpeed(double s) { m_speed = s; }
    void setCallsign(const std::string& s) { m_callsign = s; }
    
    sf::Color getColor() const;
    const std::deque<TrackPoint>& getHistory() const { return m_history; }
    
    // Factory methods for realistic spawning
    static Target createAirliner(int id, double maxRange);
    static Target createFighter(int id, double maxRange);
    static Target createMissile(int id, double maxRange);
    static Target createShip(int id, double maxRange);

private:
    void updatePos(double dt);
    void calcPolar();

    int m_id;
    std::string m_callsign;
    TargetType m_type;
    TargetStatus m_status;
    
    double m_x, m_y, m_alt;
    double m_speed, m_heading;
    double m_range, m_bearing;
        
    bool m_detected = false;
    bool m_active = true;
    std::deque<TrackPoint> m_history;
    double m_historyTimer = 0;
};

#endif