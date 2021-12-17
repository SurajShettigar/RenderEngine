#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <memory>
#include <vector>

#include "../utils/hitinfo.h"
#include "../material/material.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;

namespace raytracer
{
    class Geometry
    {
    protected:
        shared_ptr<Material> m_material;
    public:
        const shared_ptr<Material>& material = m_material;
        
        Geometry() = default;
        Geometry(shared_ptr<Material> material)
         : m_material(material) {}

        virtual bool isHit(const Ray &ray, double tMin, double tMax, HitInfo &hitInfo) const = 0;
    };
}

#endif