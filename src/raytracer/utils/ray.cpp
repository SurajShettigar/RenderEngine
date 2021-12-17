#include "ray.h"

using namespace raytracer;

Ray &Ray::operator=(const Ray &ray)
{
    m_origin = ray.origin;
    m_direction = ray.direction;
    return *this;
}

Point Ray::getPointAtDistance(double t) const
{
    return m_origin + t * m_direction;
}