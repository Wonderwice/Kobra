#include "camera/camera.h"
#include "cobra.h"

namespace cobra
{
    // ----------------------------
    // Constructors & Destructors
    // ---------------------------

    camera::camera(const size_t width, const float aspect_ratio, float viewport_height, const float focal_length) : width(width)
    {
        height = int(width / aspect_ratio);
        height = (height < 1) ? 1 : height;

        camera_center = lookfrom;

        auto focal_len = (lookfrom - lookat).length();
        double theta = degrees_to_radians(vfov);
        double h = std::tan(theta / 2);
        viewport_height = 2 * h * focal_len;
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
        auto viewport_upper_left = camera_center - (focal_length * w) - viewport_u/2 - viewport_v/2;
        pixel00 = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    camera::~camera()
    {
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

        auto ray_direction = pixel_sample - camera_center;

        return ray(camera_center, ray_direction);
    }
}