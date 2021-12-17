#include "mesh.h"

using namespace raytracer;

bool Mesh::isHit(const Ray &ray, double tMin, double tMax, HitInfo &hitInfo) const
{
    HitInfo tempHitInfo;
    for (const Triangle &t : m_triangles)
    {
        if (t.isHit(ray, tMin, tMax, tempHitInfo))
        {
            hitInfo = tempHitInfo;
            return true;
        }
    }

    return false;
}