#ifndef RADAR_SYSTEM_H
#define RADAR_SYSTEM_H

#include <vector>
#include "Target.h"

class RadarSystem {
public:
    RadarSystem();
    
    void init();
    void update(double dt);
    
    const std::vector<Target>& getTargets() const { return m_targets; }
    Target* getTarget(int id);
    
    double getSweepAngle() const { return m_sweepAngle; }
    double getMaxRange() const { return m_maxRange; }
    double getTime() const { return m_time; }
    
    // Specific spawn functions
    void spawnAirliner();
    void spawnFighter();
    void spawnMissile();
    void spawnShip();
    void clearTargets();
    
private:
    void updateSweep(double dt);
    void detectTargets();
    
    std::vector<Target> m_targets;
    double m_sweepAngle = 0;
    double m_maxRange = 10000.0; // 10,000 km
    double m_time = 0;
};

#endif