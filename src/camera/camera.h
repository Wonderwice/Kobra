#pragma once
#include "core/ray.h"
#include "core/vec3.h"

namespace cobra
{
    /**
     * @class camera
     * @brief Represents a 3D camera for ray generation.
     *
     * The camera defines the viewport and allows generation of rays
     * corresponding to image plane coordinates for ray tracing.
     */
    class camera
    {
    private:
        size_t width;           ///< Image width in pixels
        size_t height;          ///< Image height in pixels
        double viewport_width;  ///< Viewport width in world units
        double viewport_height; ///< Viewport height in world units
        vec3 pixel_delta_u;     ///< Vector step for moving one pixel horizontally
        vec3 pixel_delta_v;     ///< Vector step for moving one pixel vertically
        vec3 pixel00;           ///< Position of the top-left pixel in world space
        vec3 camera_center;     ///< Camera position in world space

        /**
         * @brief Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
         */
        vec3 sample_square() const;

    public:
        /**
         * @brief Constructs a camera with given parameters.
         * @param width Image width in pixels.
         * @param aspect_ratio Aspect ratio of the viewport (width / height).
         * @param viewport_height Height of the viewport in world units.
         * @param focal_length Distance from camera center to viewport plane.
         */
        camera(const size_t width, const float aspect_ratio, float viewport_height, const float focal_length);

        /// Default destructor.
        ~camera();

        /// @brief Get image width in pixels.
        size_t image_width() const { return width; }

        /// @brief Get image height in pixels.
        size_t image_height() const { return height; }

        /**
         * @brief Generate a ray from the camera passing through the viewport at coordinates (u,v).
         * @param u Horizontal coordinate normalized between 0 and 1.
         * @param v Vertical coordinate normalized between 0 and 1.
         * @return Ray originating at camera center through pixel (u,v).
         */
        const ray generate_ray(const double u, const double v) const;
    };
}
