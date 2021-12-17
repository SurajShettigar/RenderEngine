#ifndef CAMERA_H
#define CAMERA_H

#include "../../math/math.h"
#include "ray.h"

namespace raytracer
{
    using math::Point;

    class Camera
    {
    private:
        float m_fov = 60.0;
        float m_aspectRatio = 16.0 / 9.0;
        float m_focusDistance = 1.0;
        float m_aperture = 0.0;

        Point m_position = Point::zero;
        Point m_lookAtPosition = Point::back;
        Vector3 m_up = Vector3::up;

        Vector3 w = (m_position - m_lookAtPosition).normalize();
        Vector3 u = Vector3::cross(m_up, w).normalize();
        Vector3 v = Vector3::cross(w, u);

        float m_viewportHeight = 1.125;
        float m_viewportWidth = 2.0;

        Point lowerLeftCorner = m_position 
                                - (m_focusDistance * u * m_viewportWidth / 2.0) 
                                - (m_focusDistance * v * m_viewportHeight / 2.0) 
                                - (w * m_focusDistance);

    public:
        const float &fov = m_fov;
        const float &aspectRatio = m_aspectRatio;
        const float &focusDistance = m_focusDistance;
        const float &aperture = m_aperture;

        const float &viewportHeight = m_viewportHeight;
        const float &viewportWidth = m_viewportWidth;

        Point &position = m_position;
        Point &lookAtPosition = m_lookAtPosition;
        Vector3 &up = m_up;

        Camera(float fov = 60.0,
               float aspectRatio = 1.77778,
               float focusDistance = 1.0,
               float aperture = 0.0,
               Point position = Point::zero,
               Point lookAtPosition = Point::zero,
               Vector3 up = Vector3::up)
            : m_fov{fov},
              m_aspectRatio{aspectRatio},
              m_focusDistance{focusDistance},
              m_aperture{aperture},
              m_position{position},
              m_lookAtPosition{lookAtPosition},
              m_up{up},
              m_viewportHeight{static_cast<float>(2.0 * tan(math::degreeToRadians(fov) / 2.0))},
              m_viewportWidth{m_aspectRatio * m_viewportHeight} {}

        // Returns a ray from the given uv co-ordinates
        Ray getRay(double x, double y) const
        {
            Vector3 offset = Vector3::randomCircular() * (m_aperture / 2.0);
            // Orient the offset x-y plane to match camera orientation
            offset = offset * u;
            return Ray(m_position + offset,
                       lowerLeftCorner 
                       + x * u * m_focusDistance * m_viewportWidth 
                       + y * v * m_focusDistance * m_viewportHeight 
                       - m_position 
                       - offset);
        }
    };
}

#endif