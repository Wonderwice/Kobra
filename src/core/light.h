#pragma once
#include "core/material.h"
#include "core/texture.h"

namespace cobra
{
    /**
     * @class diffuse_light
     * @brief Material that emits light rather than reflecting it.
     *
     * This material represents a light source. It does not scatter rays;
     * instead, it returns a color when its `emitted` function is called.
     * The light emission can be constant (solid color) or textured.
     */
    class diffuse_light : public material
    {
    public:
        /**
         * @brief Constructs a diffuse light material using a texture.
         * @param tex A shared pointer to a texture representing light emission.
         */
        diffuse_light(shared_ptr<texture> tex) : tex(tex) {}

        /**
         * @brief Constructs a diffuse light material using a constant color.
         * @param emit A vector representing the RGB emission color.
         */
        diffuse_light(const vec3 &emit)
            : tex(make_shared<solid_color>(emit)) {}

        /**
         * @brief Returns the emitted color from the material at a point.
         *
         * This overrides the default implementation of emitted in `material`.
         * Since this material emits light, the result is the color defined by the texture.
         *
         * @param u Texture coordinate u.
         * @param v Texture coordinate v.
         * @param p World-space point of emission.
         * @return The emitted color at the point.
         */
        vec3 emitted(const ray &r_in, const hit_record &rec, double u, double v, const vec3 &p) const override
        {
            if (!rec.front_face)
                return vec3(0, 0, 0);
            return tex->value(u, v, p);
        }

    private:
        shared_ptr<texture> tex; ///< Texture that defines the emission color.
    };
} // namespace cobra
