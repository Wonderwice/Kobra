#pragma once

#include "core/material.h"
#include "cobra.h"
#include "core/hit_record.h"
#include "core/texture.h"
#include <memory>
#include "core/onb.h"

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
         * @param pdf The value of the pdf, for importance sampling.
         * @return true Always returns true for Lambertian surfaces.
         */
        bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered, double& pdf)
            const override
        {
            onb unw(rec.normal);
            auto scatter_direction = unw.transform(random_cosine_direction());
            
            scattered = ray(rec.point, unit_vector(scatter_direction));
            attenuation = tex->value(rec.u, rec.v, rec.point);
            pdf = dot(unw.w(), scattered.get_direction()) / pi;
            return true;
        }

        double scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered)
            const override
        {
            auto cos_theta = dot(rec.normal, unit_vector(scattered.get_direction()));
            return cos_theta < 0 ? 0 : cos_theta / pi;
        }
    };
}
