#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>

namespace math
{
    constexpr double INIFINITY = std::numeric_limits<double>().infinity();
    constexpr double PI = 3.1415926535897932385;

    inline double degreeToRadians(double degrees)
    {
        return degrees * PI / 180.0;
    }

    inline double radiansToDegrees(double radians)
    {
        return radians * 180.0 / PI;
    }

    inline double clamp(double x, double xMin, double xMax)
    {
        return x < xMin ? xMin : (x > xMax ? xMax : x);
    }

    // Random number in the range of [0, 1)
    inline double random()
    {
        return static_cast<double>(rand()) / (RAND_MAX + 1.0);
    }

    // Random number in the range of [min, max)
    inline double random(double min, double max)
    {
        return min + (max - min) * random();
    }

    // Maps a number x from its range [xMin, xMax] to output range [outMin, outMax]
    inline double mapRange(double x, double xMin, double xMax, double outMin, double outMax)
    {
        return outMin + (x - xMin) * (outMax - outMin) / (xMax - xMin);
    }
}

#endif