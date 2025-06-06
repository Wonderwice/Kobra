#include "core/material.h"
#include "cobra.h"
#include "core/hit_record.h"

namespace cobra
{
    /**
     * @class metal
     * @brief Represents a metallic material with reflective properties.
     *
     * This material models specular reflection similar to that of polished metals.
     * The `fuzz` factor introduces imperfection to the reflection, simulating brushed or rough surfaces.
     */
    class metal : public material
    {
    private:
        vec3 albedo; ///< The base color of the metal surface.
        double fuzz; ///< The fuzziness factor (0 = perfect mirror, >0 = rougher reflection).

    public:
        /**
         * @brief Constructs a metal material with given albedo and fuzziness.
         * @param albedo The reflective color of the metal.
         * @param fuzz The fuzziness factor (clamped to [0, 1] in practice).
         */
        metal(const vec3 &albedo, double fuzz) : albedo(albedo), fuzz(fuzz) {}

        /**
         * @brief Computes the reflection of the incoming ray on a metallic surface.
         *
         * @param r_in The incoming ray.
         * @param rec The hit record containing the intersection details.
         * @param attenuation The attenuation color, equal to the albedo.
         * @param scattered The resulting scattered ray.
         * @param pdf The value of the pdf, for importance sampling.
         * @return true if the scattered ray is reflected in the correct direction (i.e., not inside the object).
         */
        bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override
        {
            vec3 reflected = reflect(r_in.get_direction(), rec.normal);

            srec.attenuation = albedo;
            srec.pdf_ptr = nullptr;
            srec.skip_pdf = true;
            srec.skip_pdf_ray = ray(rec.point, reflected);

            return true;
        }
    };
}
