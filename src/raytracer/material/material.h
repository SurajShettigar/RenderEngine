#ifndef MATERIAL_H
#define MATERIAL_H

#include "../utils/hitinfo.h"

#include <cmath>

using math::Color;

namespace raytracer
{
    class Material
    {
    protected:
        Color m_color;

    public:
        Color &color = m_color;

        Material(Color color)
            : m_color{color} {}

        virtual bool scatterRay(const Ray &rayIn, const HitInfo &hitInfo, Color &atten, Ray &rayOut) const = 0;
    };
}

#endif