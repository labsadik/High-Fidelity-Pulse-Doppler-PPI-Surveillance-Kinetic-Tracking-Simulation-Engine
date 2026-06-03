#include "RadarSystem.h"
#include "Utils.h"

RadarSystem::RadarSystem() {}

void RadarSystem::init() {
    // Start with a mix of realistic traffic
    for(int i=0; i<5; i++) spawnAirliner();
    for(int i=0; i<2; i++) spawnFighter();
    for(int i=0; i<2; i++) spawnShip();
    spawnMissile(); // Start with one threat
}

void RadarSystem::update(double dt) {
    m_time += dt;
    updateSweep(dt);
    
    for (auto& t : m_targets) {
        t.update(dt);
        
        // Respawn logic
        if (t.getRange() > m_maxRange * 1.5) {
             // If missile goes out of range, delete it or respawn
             if(t.getType() == TargetType::MISSILE) {
                 t.m_active = false; 
             } else {
                 // Respawn commercial traffic
                 double r = Utils::randomDouble(m_maxRange * 0.6, m_maxRange * 0.9);
                 double b = Utils::randomDouble(0, 360);
                 double x, y;
                 Utils::polarToCartesian(r, b, x, y);
                 t.setPosition(x, y);
                 t.setDetected(false);
             }
        }
    }
    
    // Clean up inactive missiles
    m_targets.erase(std::remove_if(m_targets.begin(), m_targets.end(), 
        [](const Target& t){ return !t.isActive(); }), m_targets.end());

    detectTargets();
}

void RadarSystem::updateSweep(double dt) {
    m_sweepAngle += 36.0 * dt; 
    if (m_sweepAngle >= 360.0) m_sweepAngle -= 360.0;
}

void RadarSystem::detectTargets() {
    for (auto& t : m_targets) {
        double diff = std::abs(t.getBearing() - m_sweepAngle);
        if (diff > 180) diff = 360 - diff;
        
        if (diff < 5.0) {
            t.setDetected(true);
        }
    }
}

Target* RadarSystem::getTarget(int id) {
    for (auto& t : m_targets) if (t.getId() == id) return &t;
    return nullptr;
}

void RadarSystem::spawnAirliner() { m_targets.push_back(Target::createAirliner(m_targets.size(), m_maxRange)); }
void RadarSystem::spawnFighter()  { m_targets.push_back(Target::createFighter(m_targets.size(), m_maxRange)); }
void RadarSystem::spawnMissile()  { m_targets.push_back(Target::createMissile(m_targets.size(), m_maxRange)); }
void RadarSystem::spawnShip()     { m_targets.push_back(Target::createShip(m_targets.size(), m_maxRange)); }

void RadarSystem::clearTargets() { m_targets.clear(); }