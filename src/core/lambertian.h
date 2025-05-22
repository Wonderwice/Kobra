#pragma once

#include "core/material.h"
#include "cobra.h"
#include "core/hit_record.h"
#include "core/texture.h"
#include <memory>

namespace cobra
{
    /**
     * @class lambertian
     * @brief Represents a Lambertian (diffuse) material.
     *
     * This material models perfectly diffuse reflection using Lambert's cosine law.
     * When a ray hits a Lambertian surface, it is scattered in a random direction
     * biased by the surface normal.
     */
    class lambertian : public material
    {
    private:
        shared_ptr<texture> tex; ///< The surface texture.
    public:
        /**
         * @brief Constructs a Lambertian material with the given albedo.
         * @param albedo The color that the surface reflects.
         */
        lambertian(const vec3 &albedo) : tex(make_shared<solid_color>(albedo)) {}
        lambertian(std::shared_ptr<texture> tex) : tex(tex) {}

        /**
         * @brief Determines how the incoming ray is scattered upon hitting the surface.
         *
         * @param r_in The incoming ray.
         * @param rec The hit record containing intersection details.
         * @param attenuation The color attenuation (set to the albedo).
         * @param scattered The scattered ray from the surface.
         * @return true Always returns true for Lambertian surfaces.
         */
        bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered)
            const override
        {
            auto scatter_direction = rec.normal + random_unit_vector();
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;
            scattered = ray(rec.point, scatter_direction);
            attenuation = tex->value(rec.u, rec.v, rec.point);
            return true;
        }
    };
}
