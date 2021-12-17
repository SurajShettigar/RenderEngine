#ifndef SPHERE_H
#define SPHERE_H

#include "geometry.h"

namespace raytracer
{
    class Sphere : public Geometry
    {
    private:
        float m_radius = 1.0;
        Point m_origin = Point::zero;
    public:
        float& radius = m_radius;
        Point& origin = m_origin;

        Sphere(float radius, Point origin, shared_ptr<Material> material)
            : Geometry(material), m_radius{radius}, m_origin{origin} {}

        bool isHit(const Ray& ray, double tMin, double tMax, HitInfo& hitInfo) const override;
    };
}

#endif