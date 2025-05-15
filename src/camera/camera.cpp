#include "camera/camera.h"

namespace cobra
{
    // ----------------------------
    // Constructors & Destructors
    // ---------------------------

    camera::camera(const size_t width, const float aspect_ratio, float viewport_height, const float focal_length) : width(width)
    {
        height = int(width / aspect_ratio);
        height = (height < 1) ? 1 : height;
        viewport_width = viewport_height * (double(width) / height);
        camera_center = point3(0, 0, 0);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / width;
        pixel_delta_v = viewport_v / height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00 = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    camera::~camera()
    {
    }

    // ----------------------------
    // Ray generation
    // ---------------------------

    const ray camera::generate_ray(const double u, const double v) const
    {
        vec3 pixel_pos = pixel00 + u * pixel_delta_u + v * pixel_delta_v;
        vec3 direction = pixel_pos - camera_center;
        return ray(camera_center, direction);
    }
}