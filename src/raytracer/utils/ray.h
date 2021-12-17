#ifndef RAY_H
#define RAY_H

#include "../../math/math.h"

namespace raytracer
{
    using math::Point;
    using math::Vector3;

    class Ray
    {
    private:
        Point m_origin;
        Vector3 m_direction;

    public:
        Ray()
        : m_origin {Vector3::zero}, m_direction {Vector3::forward} {}
        Ray(const Point &origin, const Vector3 &direction)
            : m_origin{origin}, m_direction{direction} {}

        Ray& operator=(const Ray& ray);

        Point &origin = m_origin;
        Vector3 &direction = m_direction;

        Point getPointAtDistance(double dist) const;
    };
}

#endif