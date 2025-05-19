#include <iostream>
#include "renderer/raytracer.h"
#include "cobra.h"
#include "camera/camera.h"
#include "geometry/sphere.h"

namespace cobra
{
    raytracer::raytracer(const size_t nb_samples, const size_t depth, const camera &cam, const scene &_scene) : nb_samples(nb_samples), depth(depth), cam(cam), _scene(_scene)
    {
        width = cam.image_width();
        height = cam.image_height();
    }

    raytracer::~raytracer()
    {
    }

    image raytracer::render_image()
    {
        image img_result(width, height);

        for (size_t j = 0; j < height; ++j)
        {
            for (size_t i = 0; i < width; ++i)
            {
                vec3 final_color(0, 0, 0);

                for(size_t sample = 0; sample < nb_samples; ++sample){
                    vec3 color_contrib = trace_ray(cam.generate_ray(i,j), _scene, depth);
                    final_color += color_contrib;
                }
                final_color /= nb_samples;
                img_result.set_pixel(j,i, final_color);
            }
        }
        return img_result;
    }

    vec3 raytracer::trace_ray(const ray &r, const scene &scene, const size_t depth)
    {
        if(depth <= 0)
            return vec3(0,0,0);
            
        hit_record closest_hit;
        double closest_so_far = std::numeric_limits<double>::infinity();
        bool hit_anything = false;

        for (const auto &obj : scene.get_hittables())
        {
            hit_record temp_hit;
            if (obj->hit(r, 0.001, closest_so_far, temp_hit))
            {
                hit_anything = true;
                closest_so_far = temp_hit.t;
                closest_hit = temp_hit;
            }
        }

        if (hit_anything)
        {
            vec3 direction = closest_hit.normal + random_unit_vector();
            return 0.5 * trace_ray(ray(closest_hit.point,direction),scene,depth-1);
        }

        auto t = 0.5 * (r.get_direction().y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }

    double raytracer::fRand(double fMin, double fMax)
    {
        double f = (double)rand() / RAND_MAX;
        return fMin + f * (fMax - fMin);
    }
}