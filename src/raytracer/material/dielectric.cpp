#include "dielectric.h"

using raytracer::Dielectric;

bool Dielectric::scatterRay(const Ray &rayIn, const HitInfo &hitInfo, math::Color &atten, Ray &rayOut) const
{

    Vector3 dir = refract(rayIn.direction.normalize(),
                          hitInfo.normal,
                          hitInfo.isFrontFace
                              ? m_ior
                              : 1.0 / m_ior);

    rayOut = Ray(hitInfo.point, dir);
    atten = Color::one;

    return true;
}

/**
 * Snell's law for refraction:
 * i*sinθ=i'*sinθ', where i is refractive index of incident ray, θ is the angle
 * of incident ray R to the surface normal n and i', θ' are that of the 
 * refracted ray R' with normal n'
 * 
 * Refracted vector R' can be split into Rx' (vector perpendicular to normal n')
 * and Ry' (vector parallel to normal n')
 * R' = Rx' + Ry'
 * 
 * By solving the snell's law we get,
 * Rx' = (i/i') * (R + cosθ * n)
 * cosθ = -R•n
 * Rx' = (i/i') * (R + (-R•n) * n)
 * Ry' = -1 * sqrt(1 - lengthsquared(Rx')) * n
 * 
 * i = 1.0 (Refractive index of air)
 */
Vector3 Dielectric::refract(const Vector3 &v, const Vector3 &normal, double ior) const
{
    double cosTheta = fmin(Vector3::dot(-v, normal), 1.0);
    double sinTheta = sqrt(1 - cosTheta * cosTheta);
    double iorRatio = 1.0 / ior;

    Vector3 refractX = iorRatio * (v + cosTheta * normal);
    Vector3 refractY = -sqrt(fabs(1 - refractX.lengthSquared())) * normal;

    bool isReflectedRay = iorRatio * sinTheta > 1.0;
    if (isReflectedRay || (reflectance(cosTheta, iorRatio) > math::random()))
        return Vector3::reflect(v, normal);
    else
        return refractX + refractY;
}

// Schlick's approximation for reflectance.
double Dielectric::reflectance(double cosTheta, double iorRatio) const
{
    double r = (1 - iorRatio) / (1 + iorRatio);
    r = r * r;
    return r + (1.0 - r) * pow((1 - cosTheta), 5.0);
}