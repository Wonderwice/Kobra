#pragma once
#include "core/ray.h"
#include "core/vec3.h"
#include "image/image.h"
#include "scene/scene.h"

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
        double viewport_width;  ///< Viewport width in world units
        double viewport_height; ///< Viewport height in world units

        vec3 pixel_delta_u;  ///< Vector step for moving one pixel horizontally
        vec3 pixel_delta_v;  ///< Vector step for moving one pixel vertically
        vec3 pixel00;        ///< Position of the top-left pixel in world space
        vec3 camera_center;  ///< Camera position in world space
        vec3 u, v, w;        ///< Camera frame basis vectors
        vec3 defocus_disk_u; ///< Defocus disk horizontal radius
        vec3 defocus_disk_v; ///< Defocus disk vertical radius

        /**
         * @brief Generate a random double in the range [fMin, fMax].
         * @param fMin Minimum value.
         * @param fMax Maximum value.
         * @return Random double between fMin and fMax.
         */
        double fRand(double fMin, double fMax);

        /**
         * @brief Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
         */
        vec3 sample_square() const;
        /**
         * @brief Returns the vector to a random point in the defocus disk
         */
        vec3 defocus_disk_sample() const;

    public:
        size_t width = 400;             ///< Image width in pixels
        size_t height;                  ///< Image height in pixels
        double aspect_ratio = 1.; ///< Aspect ratio.
        size_t nb_samples = 10;         ///< Number of samples for anti-aliasing.
        size_t depth = 10;              ///< Number of rebound for a primary ray.

        double vfov = 90;               ///< Vertical view angle (field of view)
        vec3 lookfrom = vec3(0,0,0); ///< Point camera is looking from
        vec3 lookat = vec3(0, 0, -1);    ///< Point camera is looking at
        vec3 vup = vec3(0, 1, 0);       ///< Camera-relative "up" direction
        double defocus_angle = 0;     ///< Variation angle of rays through each pixel
        double focus_dist = 10;         ///< Distance from camera lookfrom point to plane of perfect focus

        /**
         * @brief Constructs a camera.
         */
        camera();

        /// Default destructor.
        ~camera();

        /// @brief Initialize the empty params.
        void init();

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

        /**
         * @brief Render the scene and produce the image.
         * @return Rendered image.
         */
        image render_image(const scene &world);

        /**
         * @brief Trace a ray through the scene to compute its color.
         * @param r Ray to trace.
         * @param scene Scene to trace in.
         * @param depth Current recursion depth.
         * @return Computed color as vec3.
         */
        vec3 trace_ray(const ray& r, const scene& scene, const size_t depth);
    };
}
