#ifndef LAMBERT_H
#define LAMBERT_H

#include "material.h"

namespace raytracer
{
    class Lambert : public Material
    {
    protected:
    public:
        Lambert(Color albedo)
            : Material(albedo) {}

        bool scatterRay(const Ray &rayIn, const HitInfo &hitInfo, math::Color &atten, Ray &rayOut) const override;
    };
}

#endif