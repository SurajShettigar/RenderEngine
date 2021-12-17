#include "lambert.h"

using raytracer::Lambert;

bool Lambert::scatterRay(const Ray &rayIn, const HitInfo &hitInfo, math::Color &atten, Ray &rayOut) const
{
    Vector3 dir = hitInfo.normal + Vector3::randomSpherical();

    if (dir == Vector3::zero)
        dir = hitInfo.normal;

    rayOut = Ray(hitInfo.point, dir);
    atten = m_color;

    return true;
}