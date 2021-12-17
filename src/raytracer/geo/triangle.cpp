#include "triangle.h"

using namespace raytracer;

/**
 * Moller-Trumbore triangle-ray intersection algorithm
 */
bool Triangle::isHit(const Ray &ray, double tMin, double tMax, HitInfo &hitInfo) const
{
    if (m_vertices.size() < 3 || m_vertices.size() > 3)
        return false;
    
    double epsilon = 0.0001;

    // Compute Normal
    Vector3 edgeDir1 = m_vertices[1] - m_vertices[0];
    Vector3 edgeDir2 = m_vertices[2] - m_vertices[0];

    Vector3 h = Vector3::cross(ray.direction, edgeDir2);
    double a = Vector3::dot(edgeDir1, h);

    if (a > -epsilon && a < -epsilon)
        return false;

    double f = 1.0 / a;
    Vector3 s = ray.origin - m_vertices[0];
    double u = f * Vector3::dot(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    Vector3 q = Vector3::cross(s, edgeDir1);
    double v = f * Vector3::dot(ray.direction, q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    float t = f * Vector3::dot(edgeDir2, q);
    if (t < epsilon)
        return false;
    else
    {
        hitInfo.point = ray.getPointAtDistance(t);
        hitInfo.distInRay = t;
        Vector3 normal = (m_normals[0] + m_normals[1] + m_normals[2]) / 3.0;
        hitInfo.setFaceNormal(ray.direction, normal.normalize());
        hitInfo.material = m_material;

        return true;
    }
}