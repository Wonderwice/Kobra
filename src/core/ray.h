#pragma once
#include "core/vec3.h"

namespace cobra
{
    /**
     * @brief Represents a ray in 3D space defined by an origin point and a direction vector.
     * 
     * The direction vector is expected to be normalized for most ray tracing calculations.
     */
    class ray
    {
    private:
        vec3 origin;     ///< Starting point of the ray
        vec3 direction;  ///< Direction vector of the ray

    public:
        /**
         * @brief Default constructor for a ray.
         */
        ray();

        /**
         * @brief Constructs a ray given an origin and a direction.
         * 
         * @param origin The starting point of the ray.
         * @param direction The direction vector of the ray (ideally normalized).
         */
        ray(const vec3& origin, const vec3& direction);

        /**
         * @brief Destructor.
         */
        ~ray();

        /**
         * @brief Get the origin of the ray.
         * @return The origin point as a const reference.
         */
        const vec3& get_origin() const;

        /**
         * @brief Get the direction of the ray.
         * @return The direction vector as a const reference.
         */
        const vec3& get_direction() const;

        /**
         * @brief Compute the position along the ray at parameter t.
         * 
         * Point = origin + t * direction
         * 
         * @param t The parameter along the ray.
         * @return The 3D point at parameter t.
         */
        vec3 at(double t) const;

        ray& operator=(const cobra::ray&);
    };
}
