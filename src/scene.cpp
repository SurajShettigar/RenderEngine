#include "scene.h"

raytracer::GeometryList Scene::generateSceneFromModel(raytracer::Mesh mesh)
{

    using namespace raytracer;

    shared_ptr<Material> groundMat = make_shared<Lambert>(Color::one * 0.5);

    GeometryList geoList;
    // Create ground
    geoList.add(make_shared<Sphere>(1000.0, Point(0.0, -1000.0, 0.0), groundMat));

    shared_ptr<Mesh> meshPtr = make_shared<Mesh>(mesh);
    geoList.add(meshPtr);

    m_currenGeoList = geoList;

    return geoList;
}

raytracer::GeometryList Scene::generateRandomScene()
{
    using namespace raytracer;

    shared_ptr<Material> groundMat = make_shared<Lambert>(Color::one * 0.5);

    GeometryList geoList;

    // Create ground
    geoList.add(make_shared<Sphere>(1000.0, Point(0.0, -1000.0, 0.0), groundMat));
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            double randMat = math::random();
            shared_ptr<Material> mat = groundMat;
            if (randMat < 0.2)
                mat = make_shared<Dielectric>(Color::one, 1.51);
            else if (randMat < 0.4)
                mat = make_shared<Metallic>(Color::random(0.8, 1.0), math::random(0.025, 0.65));
            else
                mat = make_shared<Lambert>(Color::random(0.3, 1.0));

            double randRadius = math::random(0.25, 1.0);
            if (math::random() < 0.7)
                randRadius = math::random(0.25, 0.5);
            else
                randRadius = math::random(0.75, 1.0);

            double randXPos = math::mapRange(static_cast<double>(x), 0.0, 8.0, -10.0, 10.0);
            randXPos += math::random(-randRadius / 2.0, randRadius / 2.0);
            double randZPos = math::mapRange(static_cast<double>(y), 0.0, 8.0, 0.0, -20.0);
            randZPos += math::random(-randRadius / 2.0, randRadius / 2.0);
            Point pos = Point(randXPos, randRadius, randZPos);

            shared_ptr<Sphere> randSphere = make_shared<Sphere>(randRadius, pos, mat);
            geoList.add(randSphere);
        }
    }

    m_currenGeoList = geoList;

    return geoList;
}

Color Scene::getRayPixelColor(const raytracer::Ray &ray, const raytracer::Geometry &geo, int currBounce)
{
    raytracer::HitInfo hit;

    if (currBounce <= 0)
        return Color::zero;

    if (geo.isHit(ray, 0.0001, INFINITY, hit))
    {   
        Color color = Color::zero;
        raytracer::Ray outRay;

        if (hit.material->scatterRay(ray, hit, color, outRay))
            return color * getRayPixelColor(outRay, geo, --currBounce);
        else
            return Color::zero;
    }

    /// Color the background
    Vector3 normalizedDir = Vector3::normalize(ray.direction);
    normalizedDir = (normalizedDir + Vector3::one) / 2.0;
    return Vector3::lerp(Color(1.0, 1.0, 1.0), Color(0.5, 0.7, 1.0), normalizedDir.y);
}

void Scene::processImageColor(Color &color, int samples)
{
    color /= samples;

    // Gamma correction with gamma 2
    color.x = sqrt(color.x);
    color.y = sqrt(color.y);
    color.z = sqrt(color.z);

    color.x = math::clamp(color.x, 0.0, 0.999);
    color.y = math::clamp(color.y, 0.0, 0.999);
    color.z = math::clamp(color.z, 0.0, 0.999);
}

void Scene::renderPixels(int index, int start_x, int start_y, int end_x, int end_y, void (*callback)(uint8_t *))
{
    Color color(1.0, 1.0, 1.0);

    for (int y = start_y; y < end_y; ++y)
    {
        for (int x = start_x; x < end_x; ++x)
        {
            color = Color::zero;
            for (int s = 0; s < m_image.samplesPerPixel; s++)
            {
                double u = (x + math::random()) / (m_image.width - 1);
                double v = (m_image.height - 1 - (y + math::random())) / (m_image.height - 1);
                color += getRayPixelColor(m_camera.getRay(u, v), m_currenGeoList, m_image.maxBounces);
            }
            processImageColor(color, m_image.samplesPerPixel);
            m_pixels[index++] = static_cast<uint8_t>(color.x * 256);
            m_pixels[index++] = static_cast<uint8_t>(color.y * 256);
            m_pixels[index++] = static_cast<uint8_t>(color.z * 256);
        }
        callback(m_pixels);
    }
}

void Scene::setOnPixelsProcessedListener(void (*callback)(uint8_t *pixels))
{
    m_callback = callback;
}

void Scene::render(ThreadUsage threadUsage)
{
    int numThreads = std::thread::hardware_concurrency();
    switch (threadUsage)
    {
    case ThreadUsage::SINGLE:
        numThreads = 1;
        break;
    case ThreadUsage::HALF:
        numThreads /= 2;
        break;
    case ThreadUsage::MAX_MINUS_2:
        numThreads -= 2;
        break;
    case ThreadUsage::MAX:
    default:
        break;
    }

    int width = m_image.width;
    int height = m_image.height;
    int pixelsPerThread = width * height * static_cast<int>(m_image.colorChannels) / numThreads;

    std::thread threads[numThreads];
    int currPixel = 0;
    int currHeight = 0;
    for (size_t i = 0; i < numThreads; i++)
    {
        threads[i] = std::thread(&Scene::renderPixels, this, currPixel, 0, currHeight, width, currHeight + height / numThreads, m_callback);
        currPixel += pixelsPerThread;
        currHeight += height / numThreads;
    }

    for (size_t i = 0; i < numThreads; i++)
        threads[i].join();
}
