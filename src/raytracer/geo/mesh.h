#ifndef MESH_H
#define MESH_H

#include "triangle.h"

namespace raytracer
{
    class Mesh : public Geometry
    {
    protected:
        vector<Triangle> m_triangles;

    public:
        Mesh(shared_ptr<Material> material)
        : Geometry(material) {}
        Mesh(shared_ptr<Material> material, std::vector<Triangle> triangles)
        : Geometry(material), m_triangles{triangles} {}
        bool isHit(const Ray &ray, double tMin, double tMax, HitInfo &hitInfo) const override;
    };
}

#endif