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

        sqrt_spp = int(std::sqrt(nb_samples));
        recip_sqrt_spp = 1.0 / sqrt_spp;

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

    const ray camera::generate_ray(int i, int j, int s_i, int s_j) const
    {
        auto offset = sample_square_stratified(s_i, s_j);
        auto pixel_sample = pixel00 + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 camera::sample_square_stratified(int s_i, int s_j) const
    {
        auto px = ((s_i + random_double()) * recip_sqrt_spp) - 0.5;
        auto py = ((s_j + random_double()) * recip_sqrt_spp) - 0.5;

        return vec3(px, py, 0);
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

                for (int s_j = 0; s_j < sqrt_spp; s_j++)
                {
                    for (int s_i = 0; s_i < sqrt_spp; s_i++)
                    {
                        vec3 color_contrib = trace_ray(generate_ray(i, j, s_i, s_j), world, depth);
                        final_color += color_contrib;
                    }
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
        bool hit_anything = world.hit(r, interval(0.001, closest_so_far), closest_hit);

        if (hit_anything)
        {
            ray scattered;
            vec3 attenuation;
            double pdf_value;
            vec3 emission = closest_hit.mat->emitted(r, closest_hit, closest_hit.u, closest_hit.v, closest_hit.point);

            if (!closest_hit.mat->scatter(r, closest_hit, attenuation, scattered, pdf_value))
                return emission;

            auto on_light = vec3(random_double(213, 343), 554, random_double(227, 332));
            auto to_light = on_light - closest_hit.point;
            auto distance_squared = to_light.length_squared();
            to_light = unit_vector(to_light);

            if (dot(to_light, closest_hit.normal) < 0)
                return emission;

            double light_area = (343 - 213) * (332 - 227);
            auto light_cosine = std::fabs(to_light.y());
            if (light_cosine < 0.000001)
                return emission;

            pdf_value = distance_squared / (light_cosine * light_area);
            scattered = ray(closest_hit.point, to_light);

            double scattering_pdf = closest_hit.mat->scattering_pdf(r, closest_hit, scattered);

            return emission + (attenuation * scattering_pdf * trace_ray(scattered, world, depth - 1)) / pdf_value;
        }

        return background;
    }

    double camera::fRand(double fMin, double fMax)
    {
        double f = (double)rand() / RAND_MAX;
        return fMin + f * (fMax - fMin);
    }
}