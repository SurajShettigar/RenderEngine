#ifndef METALLIC_H
#define METALLIC_H

#include "material.h"

namespace raytracer
{
    class Metallic : public Material
    {
    protected:
        double m_roughness = 0.5;

    public:
        double &roughness = m_roughness;

        Metallic(Color color, double roughness = 0.5)
            : Material(color), m_roughness{math::clamp(roughness, 0.0, 1.0)} {}

        bool scatterRay(const Ray &rayIn, const HitInfo &hitInfo, math::Color &atten, Ray &rayOut) const override;
    };
}

#endif