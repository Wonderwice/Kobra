#include "camera/camera.h"
#include "cobra.h"
#include "camera.h"
#include "image/image.h"
#include "core/hit_record.h"
#include "scene/scene.h"
#include "core/material.h"
namespace cobra
{
    // ----------------------------
    // Constructors & Destructors
    // ---------------------------

    camera::camera()
    {
    }

    camera::~camera()
    {
    }

    void camera::init()
    {
        height = int(width / aspect_ratio);
        height = (height < 1) ? 1 : height;

        camera_center = lookfrom;

        double theta = degrees_to_radians(vfov);
        double h = std::tan(theta / 2);
        viewport_height = 2 * h * focus_dist;
        viewport_width = viewport_height * (double(width) / height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / width;
        pixel_delta_v = viewport_v / height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = camera_center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00 = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    // ----------------------------
    // Ray generation
    // ---------------------------

    vec3 camera::sample_square() const
    {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    const ray camera::generate_ray(const double u, const double v) const
    {
        auto offset = sample_square();
        auto pixel_sample = pixel00 + ((u + offset.x()) * pixel_delta_u) + ((v + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();

        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 camera::defocus_disk_sample() const
    {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    image camera::render_image(const scene &world)
    {
        init();
        image img_result(width, height);

#pragma omp parallel for schedule(dynamic, 1)
        for (size_t j = 0; j < height; ++j)
        {
            for (size_t i = 0; i < width; ++i)
            {
                vec3 final_color(0, 0, 0);

                for (size_t sample = 0; sample < nb_samples; ++sample)
                {
                    vec3 color_contrib = trace_ray(generate_ray(i, j), world, depth);
                    final_color += color_contrib;
                }
                final_color /= nb_samples;
                img_result.set_pixel(j, i, final_color);
            }
        }
        return img_result;
    }

    vec3 camera::trace_ray(const ray &r, const scene &world, const size_t depth)
    {
        if (depth <= 0)
            return vec3(0, 0, 0);

        hit_record closest_hit;
        double closest_so_far = std::numeric_limits<double>::infinity();
        bool hit_anything = world.hit(r,interval(0.001,closest_so_far), closest_hit);

        if (hit_anything)
        {
            ray scattered;
            vec3 attenuation;
            if (closest_hit.mat->scatter(r, closest_hit, attenuation, scattered))
                return attenuation * trace_ray(scattered, world, depth - 1);
            return vec3(0, 0, 0);
        }

        auto t = 0.5 * (r.get_direction().y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }

    double camera::fRand(double fMin, double fMax)
    {
        double f = (double)rand() / RAND_MAX;
        return fMin + f * (fMax - fMin);
    }
}