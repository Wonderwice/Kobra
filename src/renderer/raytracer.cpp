#include "renderer/raytracer.h"
#include "math/ray.h"
#include "camera/camera.h"

namespace cobra
{
    raytracer::raytracer(const size_t nb_samples, const size_t depth, const camera cam, const scene _scene) : nb_samples(nb_samples), depth(depth), cam(cam), _scene(_scene)
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
        srand(time(NULL));

        for (size_t row = 0; row < width; ++row)
        {
            for (size_t col = 0; col < height; ++col)
            {
                vec3 final_color(0, 0, 0);
                for (size_t sample = 0; sample < nb_samples; ++sample)
                {
                    double u = (row + fRand(0.0, 1.0)) / (width - 1);
                    double v = (col + fRand(0.0, 1.0)) / (height - 1);
                    vec3 color_contrib = trace_ray(cam.generate_ray(u, v), _scene, depth);
                    final_color += color_contrib;
                }
                final_color /= nb_samples;
                img_result.set_pixel(row, col, final_color);
            }
        }
        return img_result;
    }

    vec3 raytracer::trace_ray(const ray r, const scene scene, const size_t depth)
    {
        vec3 unit_direction = unit_vector(r.get_direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);
    }

    double raytracer::fRand(double fMin, double fMax)
    {
        double f = (double)rand() / RAND_MAX;
        return fMin + f * (fMax - fMin);
    }
}