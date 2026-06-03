#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <random>
#include <string>
#include <sstream>
#include <iomanip>

namespace Utils {
    constexpr double PI = 3.14159265358979323846;

    inline double randomDouble(double min, double max) {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    inline int randomInt(int min, int max) {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    inline double degToRad(double deg) { return deg * PI / 180.0; }
    inline double radToDeg(double rad) { return rad * 180.0 / PI; }

    inline void cartesianToPolar(double x, double y, double& range, double& bearing) {
        range = std::sqrt(x * x + y * y);
        bearing = radToDeg(std::atan2(x, -y));
        if (bearing < 0) bearing += 360.0;
    }

    inline void polarToCartesian(double range, double bearing, double& x, double& y) {
        double rad = degToRad(bearing);
        x = range * std::sin(rad);
        y = -range * std::cos(rad);
    }
    
    inline std::string formatTime(int seconds) {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << (seconds / 3600) << ":"
            << std::setfill('0') << std::setw(2) << ((seconds % 3600) / 60) << ":"
            << std::setfill('0') << std::setw(2) << (seconds % 60);
        return oss.str();
    }
}

#endif