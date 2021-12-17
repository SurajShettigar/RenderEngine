#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../../math/math.h"
#include "geometry.h"

using math::Color;
using math::Point;
using math::Vector3;

namespace raytracer
{
    class Triangle : public Geometry
    {
    private:
        vector<Point> m_vertices;
        vector<Vector3> m_normals;
        vector<Vector3> m_texCoords;
        vector<Color> m_vertexColors;

        int m_materialId;

    public:
        Triangle(shared_ptr<Material> material,
                 vector<Point> vertices,
                 vector<Vector3> normals,
                 vector<Vector3> texCoords,
                 vector<Color> vertexColors,
                 int materialId)
            : Geometry(material),
              m_vertices{vertices},
              m_normals{normals},
              m_texCoords{texCoords},
              m_vertexColors{vertexColors},
              m_materialId{materialId} {}

        bool isHit(const Ray &ray, double tMin, double tMax, HitInfo &hitInfo) const override;
    };
}

#endif