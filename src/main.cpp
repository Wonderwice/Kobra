#include <iostream>
#include "camera/camera.h"
#include "image/image.h"
#include "renderer/raytracer.h"
#include "scene/scene.h"
#include "image/ppm_writer.h"
#include "geometry/sphere.h"
#include <memory>
#include "core/lambertian.h"
#include "core/metal.h"
#include "core/dieletric.h"

using namespace cobra;

int main()
{
    const size_t image_width = 480;
    const double aspect_ratio = 16. / 9.;
    const size_t depth = 50;
    const size_t nb_samples = 100;

    scene _scene = scene();
    auto material_ground = std::make_shared<lambertian>(vec3(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(vec3(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<dielectric>(1.50);
    auto material_bubble = std::make_shared<dielectric>(1.00 / 1.50);
    auto material_right = std::make_shared<metal>(vec3(0.8, 0.6, 0.2), 1.0);

    _scene.add_hittable(std::make_shared<sphere>(vec3(0.0, -100.5, -1.0), 100.0, material_ground));
    _scene.add_hittable(std::make_shared<sphere>(vec3(0.0, 0.0, -1.2), 0.5, material_center));
    _scene.add_hittable(std::make_shared<sphere>(vec3(-1.0, 0.0, -1.0), 0.5, material_left));
    _scene.add_hittable(std::make_shared<sphere>(vec3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    _scene.add_hittable(std::make_shared<sphere>(vec3(1.0, 0.0, -1.0), 0.5, material_right));

    raytracer ray_tracer(nb_samples, depth, camera(image_width, aspect_ratio, 2, 1), _scene);

    const image img = ray_tracer.render_image();
    ppm_writer img_writer;

    img_writer.write(img, "../output.ppm");

    return 0;
}