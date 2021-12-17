#ifndef SCENE_H
#define SCENE_H

#include "raytracer/raytracer.h"
#include "math/math.h"

#include <thread>
#include <string>
#include <vector>

enum class ThreadUsage
{
    MAX,
    MAX_MINUS_2,
    HALF,
    SINGLE
};

class Scene
{
private:
    raytracer::GeometryList m_currenGeoList;
    raytracer::Camera m_camera;
    raytracer::Image m_image;

    uint8_t *m_pixels;

    void (*m_callback)(uint8_t *pixels);

    // raytracer::Mesh getMeshFromAttribs(tinyobj::attrib_t attribs,
    //                                    vector<tinyobj::shape_t> shapes,
    //                                    vector<tinyobj::material_t> meshMaterials);

    Color getRayPixelColor(const raytracer::Ray &ray, const raytracer::Geometry &geo, int currBounce);
    void processImageColor(Color &color, int samples);
    void renderPixels(int index, int start_x, int start_y, int end_x, int end_y, void (*callback)(uint8_t *));

public:
    Scene()
        : m_camera{raytracer::Camera(25.0,                 // FOV
                                     16.0 / 9.0,           // Aspect Ratio
                                     13.0,                 // Focus Distance
                                     0.25,                 // Aperture
                                     Point(8.0, 2.5, 7.0), // Camera position
                                     Point(0.0, 0.0, -10.0))},
          m_image{raytracer::Image(640, 360)},
          m_pixels{new uint8_t[m_image.width * m_image.height * m_image.colorChannels]}
    {
        std::fill_n(m_pixels, sizeof(m_pixels) / sizeof(uint8_t), 0);
    }
    Scene(raytracer::Camera camera, raytracer::Image image)
        : m_camera{camera},
          m_image{image},
          m_pixels{new uint8_t[m_image.width * m_image.height * m_image.colorChannels]}
    {

        std::fill_n(m_pixels, sizeof(m_pixels) / sizeof(uint8_t), 0);
    }

    ~Scene()
    {
        delete[] m_pixels;
    }

    const uint8_t &pixels = *m_pixels;

    // bool loadModelFromFile(const char *path);
    raytracer::GeometryList generateSceneFromModel(raytracer::Mesh mesh);
    raytracer::GeometryList generateRandomScene();
    void setOnPixelsProcessedListener(void (*callback)(uint8_t *pixels));
    void render(ThreadUsage threadUsage);
};

#endif