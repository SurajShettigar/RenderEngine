#include "sphere.h"

using namespace raytracer;

/**
* (x-cx)^2 + (y-cy)^2 + (z-cz)^2 = r^2 is the sphere equation. Where {x,y,z} is
* a point lying on the sphere surface. {cx, cy, cz} is the centre of the
* circle.
* In terms of vectors, it can be represented as:
* (P-C)•(P-C) = r^2, where P is a point on the surface and C is its centre.
*
* If a ray R(t) hits the sphere, it should satisfy the above equation. i.e.
* (R(t)-C)•(R(t)-C) = r^2
*
* Ray R(t) = A + tb, where A is origin, b is direction and t is the value used to
* traverse along the ray.
* 
* By substituting R(t) formula, we get:
* (A+tb-C)•(A+tb-C) = r^2
* 
* By expanding the equation we get:
*  (b•b)*t^2 + 2*b•(A-C)*t + (A-C)•(A-C) - r^2 = 0
*  <-ax^2--> + <----bx---> + <-------c------>
*
* This equation is quadratic. According to quadratic equation formula, there are
* three possible outcomes based. Square root part of the formula (Discriminant)
* can yeild:
*   1. Two soultions (Positive value inside square root) - Ray passes through
*      the sphere.
*   2. One solution (Zero) - Ray intersects on sphere surface.
*   3. No solution (Negative value) - Ray does not instersect.
*/
bool Sphere::isHit(const Ray &ray, double tMin, double tMax, HitInfo &hitInfo) const
{
    Vector3 aMinusC = ray.origin - m_origin;

    // a term is dot product of ray direction with itself, which is nothing
    // but length squared.
    double a = ray.direction.lengthSquared();

    // b=2*h, where h = b•(A-C). We can simplify the quadratic equation
    // x = (-b±sqrt(b^2-4ac))/2a as follows:
    // x = (-h±sqrt(h^2-ac))/a
    double h = Vector3::dot(aMinusC, ray.direction);
    double c = aMinusC.lengthSquared() - m_radius * m_radius;

    double discriminant = h * h - a * c;

    if (discriminant < 0)
        return false;

    double t = (-h - sqrt(discriminant)) / a;
    if (t < tMin || tMax < t)
    {
        t = (-h + sqrt(discriminant)) / a;
        if (t < tMin || tMax < t)
            return false;
    }
    hitInfo.distInRay = t;
    hitInfo.point = ray.getPointAtDistance(hitInfo.distInRay);
    Vector3 outNorm = (hitInfo.point - m_origin) / m_radius;
    hitInfo.setFaceNormal(ray.direction, outNorm);

    hitInfo.material = m_material;

    return true;
}