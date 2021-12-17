#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.h"

namespace raytracer
{
    class Dielectric : public Material
    {
    private:
        Vector3 refract(const Vector3 &v, const Vector3 &normal, double ior) const;
        double reflectance(double cosTheta, double iorRatio) const; 
    protected:
        double m_ior = 1.45;

    public:
        double &ior = m_ior;

        Dielectric(Color color, double ior = 1.45)
            : Material(color), m_ior{ior} {}

        bool scatterRay(const Ray &rayIn, const HitInfo &hitInfo, math::Color &atten, Ray &rayOut) const override;
    };
}

#endif