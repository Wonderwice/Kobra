#include <iostream>
#include "camera/camera.h"
#include "image/image.h"
#include "scene/scene.h"
#include "image/ppm_writer.h"
#include "geometry/sphere.h"
#include <memory>
#include "core/lambertian.h"
#include "core/metal.h"
#include "core/dieletric.h"

using namespace cobra;

void fill_with_spheres(scene &world)
{
    auto ground_material = std::make_shared<lambertian>(vec3(0.5, 0.5, 0.5));
    world.add_hittable(std::make_shared<sphere>(vec3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = vec3::random() * vec3::random();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    world.add_hittable(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = vec3::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add_hittable(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add_hittable(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add_hittable(make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(vec3(0.4, 0.2, 0.1));
    world.add_hittable(std::make_shared<sphere>(vec3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0);
    world.add_hittable(std::make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));
}

int main()
{
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.width = 800;
    cam.nb_samples = 30;
    cam.depth = 15;

    cam.vfov = 20;
    cam.lookfrom = vec3(13, 2, 3);
    cam.lookat = vec3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    scene _scene = scene();
    fill_with_spheres(_scene);

    const image img = cam.render_image(_scene);
    ppm_writer img_writer;

    img_writer.write(img, "../output.ppm");

    return 0;
}