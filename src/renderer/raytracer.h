#pragma once
#include <cstddef>
#include <random>

#include "image/image.h"
#include "scene/scene.h"
#include "camera/camera.h"

namespace cobra
{
    /**
     * @brief Raytracer class responsible for rendering an image of a 3D scene.
     * 
     * Uses a camera to generate rays, traces them through the scene, and produces an image.
     */
    class raytracer
    {
    private:
        size_t width;       ///< Width of the output image in pixels
        size_t height;      ///< Height of the output image in pixels
        const size_t nb_samples;  ///< Number of samples per pixel for anti-aliasing
        const size_t depth;       ///< Maximum recursion depth for ray tracing

        const camera cam;         ///< Camera used to generate rays
        const scene _scene;       ///< Scene to be rendered

        /**
         * @brief Generate a random double in the range [fMin, fMax].
         * @param fMin Minimum value.
         * @param fMax Maximum value.
         * @return Random double between fMin and fMax.
         */
        double fRand(double fMin, double fMax);

    public:
        /**
         * @brief Constructs the raytracer with rendering parameters and scene.
         * @param nb_samples Number of samples per pixel.
         * @param depth Recursion depth.
         * @param cam Camera object.
         * @param _scene Scene object.
         */
        raytracer(const size_t nb_samples, const size_t depth, const camera cam, const scene _scene);

        /**
         * @brief Destructor.
         */
        ~raytracer();

        /**
         * @brief Render the scene and produce the image.
         * @return Rendered image.
         */
        image render_image();

        /**
         * @brief Trace a ray through the scene to compute its color.
         * @param r Ray to trace.
         * @param scene Scene to trace in.
         * @param depth Current recursion depth.
         * @return Computed color as vec3.
         */
        vec3 trace_ray(const ray r, const scene scene, const size_t depth);
    };
} // namespace cobra
