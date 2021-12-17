#include <chrono>
#include <unordered_map>
using std::chrono::duration;
using std::chrono::steady_clock;

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "viewport/vulkan_renderer.h"
#include "viewport/window.h"
#include "scene.h"

#define RENDER_SILENT 1

const char* RENDER_IMAGE = "../renders/teddy_render_01.png";
const char* MODEL_FILE = "../assets/teddy.obj";

bool isRendering = false;

raytracer::Mesh getMeshFromFile(const char* path)
{
    using namespace tinyobj;
    using namespace raytracer;

    std::cout << "Loading 3D Model: " << path << std::endl;
    
    shared_ptr<Material> mat = make_shared<Dielectric>(Color::one, 1.52);

    ObjReaderConfig readerConfig;
    readerConfig.mtl_search_path = "./";

    ObjReader reader;
    if (!reader.ParseFromFile(path, readerConfig))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "Failed to load model: " << reader.Error() << std::endl;
        }
        return Mesh(mat);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "Model load warning: " << reader.Warning() << std::endl;
    }

    const attrib_t &attribs = reader.GetAttrib();
    const std::vector<shape_t> &shapes = reader.GetShapes();
    const std::vector<material_t> &materials = reader.GetMaterials();


    std::vector<Triangle> triangles;
    std::cout << "Shape Count: " << shapes.size() << std::endl;
    for (size_t s = 0; s < shapes.size(); s++)
    {
        size_t indexOffset = 0;
        std::cout << "Face Count: " << shapes[s].mesh.num_face_vertices.size() << std::endl;

        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            vector<Point> vertices;
            vector<Vector3> normals;
            vector<Vector3> texCoords;
            vector<Color> colors;

            vertices.resize(fv, Point::zero);
            normals.resize(fv, Vector3::zero);
            texCoords.resize(fv, Vector3::zero);
            colors.resize(fv, Color::one);

            for (size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];

                Point currVertex(0.0, 0.0, 0.0);
                currVertex.x = static_cast<double>(attribs.vertices[3 * size_t(idx.vertex_index) + 0]);
                currVertex.y = static_cast<double>(attribs.vertices[3 * size_t(idx.vertex_index) + 1]);
                currVertex.z = static_cast<double>(attribs.vertices[3 * size_t(idx.vertex_index) + 2]);

                Vector3 currNormal(0.0, 0.0, 0.0);
                if (idx.normal_index >= 0)
                {
                    currNormal.x = static_cast<double>(attribs.normals[3 * size_t(idx.normal_index) + 0]);
                    currNormal.y = static_cast<double>(attribs.normals[3 * size_t(idx.normal_index) + 1]);
                    currNormal.z = static_cast<double>(attribs.normals[3 * size_t(idx.normal_index) + 2]);
                }

                Vector3 currTexCoords(0.0, 0.0, 0.0);
                if (idx.texcoord_index >= 0)
                {
                    currTexCoords.x = static_cast<double>(attribs.texcoords[2 * size_t(idx.texcoord_index) + 0]);
                    currTexCoords.y = static_cast<double>(attribs.texcoords[2 * size_t(idx.texcoord_index) + 1]);
                }

                Color currColor(1.0, 1.0, 1.0);
                currColor.x = static_cast<double>(attribs.colors[3 * size_t(idx.vertex_index) + 0]);
                currColor.y = static_cast<double>(attribs.colors[3 * size_t(idx.vertex_index) + 1]);
                currColor.z = static_cast<double>(attribs.colors[3 * size_t(idx.vertex_index) + 2]);

                vertices[v] = currVertex;
                normals[v] = currNormal;
                texCoords[v] = currTexCoords;
                colors[v] = currColor;
            }

            indexOffset += fv;

            // per-face material
            int matId = shapes[s].mesh.material_ids[f];

            Triangle triangle(mat, vertices, normals, texCoords, colors, matId);
            triangles.push_back(triangle);
        }
    }

    Mesh mesh(mat, triangles);

    return mesh;
}

void onPixelsProcessed(uint8_t* pixels)
{
    
}

int renderImage()
{
    isRendering = true;

    using namespace raytracer;

    Mesh mesh = getMeshFromFile(MODEL_FILE);

    Camera camera(45.0,                    // FOV
                  16.0 / 9.0,              // Aspect Ratio
                  13.0,                    // Focus Distance
                  0.0,                    // Aperture
                  Point(0.0, 1.0, 6.0),    // Camera position
                  Point(0.0, 0.0, 0.0)); // Look At position

    Image image(640, 360);
    image.samplesPerPixel = 16;
    image.maxBounces = 6;
    image.targetImageLocation = RENDER_IMAGE;

    Scene scene(camera, image);
    scene.generateSceneFromModel(mesh);
    scene.setOnPixelsProcessedListener(onPixelsProcessed);

    auto start = steady_clock::now();
    
    std::cout << "Started rendering the scene:" << std::endl;

    scene.render(ThreadUsage::MAX_MINUS_2);

    auto end = steady_clock::now();
    duration<double> elapsed = end - start;

    std::cerr << std::endl
              << "Time taken to render: " << elapsed.count() << "s" << std::endl;

    stbi_write_png(image.targetImageLocation,
                   image.width,
                   image.height,
                   image.colorChannels,
                   &scene.pixels,
                   image.width * image.colorChannels);

    isRendering = false;

    return 0;
}

void onRenderClicked()
{
    std::cout << "Render called" << std::endl;
    if (!isRendering)
        renderImage();
}

int showViewport()
{
    VulkanRenderer renderer;
    Window window(reinterpret_cast<Renderer *>(&renderer), 1280, 720, "Render Engine");
    window.setRenderListener(onRenderClicked);
    window.show();

    return 0;
}

int main(int argc, char **argv)
{
#if RENDER_SILENT
    int status = renderImage();
#else
    int status = showViewport();
#endif

    char *input;
    std::cout << "Press any key to continue";
    std::cin >> input;

    return status;
}