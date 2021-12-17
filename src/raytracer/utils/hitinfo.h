#ifndef HIT_INFO_H
#define HIT_INFO_H

#include "../../math/math.h"
#include "ray.h"

#include <memory>

using math::Point;
using math::Vector3;

using std::shared_ptr;

namespace raytracer
{
    class Material;

    struct HitInfo
    {
        Point point = Point::zero;
        Vector3 normal = Point::zero;
        double distInRay = -1.0;
        bool isFrontFace = true;
        shared_ptr<Material> material = nullptr;

        inline void setFaceNormal(const Vector3 &rayDir, const Vector3 &outwardNormal)
        {
            isFrontFace = Vector3::dot(rayDir, outwardNormal) < 0.0;
            normal = isFrontFace ? outwardNormal : -outwardNormal;
        }
    };
}
#endif