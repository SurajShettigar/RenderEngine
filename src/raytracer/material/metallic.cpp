#include "metallic.h"

using raytracer::Metallic;

bool Metallic::scatterRay(const Ray &rayIn, const HitInfo &hitInfo, math::Color &atten, Ray &rayOut) const
{
    Vector3 dir = Vector3::reflect(rayIn.direction.normalize(), hitInfo.normal);
    dir += m_roughness * Vector3::randomSpherical();

    rayOut = Ray(hitInfo.point, dir);
    atten = m_color;

    return Vector3::dot(hitInfo.normal, rayOut.direction) > 0.0;
}