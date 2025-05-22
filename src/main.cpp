#include <iostream>
#include "camera/camera.h"
#include "image/image.h"
#include "scene/scene.h"
#include "image/ppm_writer.h"
#include "geometry/sphere.h"
#include <memory>
#include <chrono>
#include "core/lambertian.h"
#include "core/metal.h"
#include "core/dieletric.h"
#include "core/bvh_node.h"
#include "geometry/quad.h"

using namespace cobra;

const image quads()
{
    scene world;

    // Materials
    auto left_red = make_shared<lambertian>(vec3(1.0, 0.2, 0.2));
    auto back_green = make_shared<lambertian>(vec3(0.2, 1.0, 0.2));
    auto right_blue = make_shared<lambertian>(vec3(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(vec3(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<lambertian>(vec3(0.2, 0.8, 0.8));

    // Quads
    world.add_hittable(make_shared<quad>(vec3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
    world.add_hittable(make_shared<quad>(vec3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add_hittable(make_shared<quad>(vec3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add_hittable(make_shared<quad>(vec3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add_hittable(make_shared<quad>(vec3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.width = 400;
    cam.nb_samples = 100;
    cam.depth = 50;

    cam.vfov = 80;
    cam.lookfrom = vec3(0, 0, 9);
    cam.lookat = vec3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    return cam.render_image(world);
}

const image fill_with_spheres()
{
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.width = 800;
    cam.nb_samples = 5;
    cam.depth = 30;

    cam.vfov = 20;
    cam.lookfrom = vec3(13, 2, 3);
    cam.lookat = vec3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;
    scene world = scene();

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

    world = scene(make_shared<bvh_node>(world));
    return cam.render_image(world);
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::unique_ptr<image> img;

    switch (2)
    {
    case 1:
        img = std::make_unique<image>(fill_with_spheres());
        break;
    case 2:
        img = std::make_unique<image>(quads());
        break;
    }

    ppm_writer img_writer;

    img_writer.write(*img, "../output.ppm");

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Render time: " << duration.count() << " ms" << std::endl;

    return 0;
}