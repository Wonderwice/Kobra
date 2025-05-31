#pragma once
#include "core/vec3.h"

namespace cobra
{
    /**
     * @class onb
     * @brief Represents an orthonormal basis in 3D space.
     *
     * The `onb` class constructs an orthonormal basis (u, v, w) given a single input vector.
     * This is typically used to define a local coordinate system where `w` is aligned with
     * a specified normal or direction vector. The other two axes are computed to be orthogonal
     * and normalized, forming a right-handed coordinate system.
     *
     * This is useful in shading, sampling, and ray generation when transforming between
     * world and local coordinate spaces.
     */
    class onb
    {
    public:
        /**
         * @brief Constructs an orthonormal basis from a given normal vector.
         *
         * @param n A non-zero vector that will become the w-axis of the new basis.
         */
        onb(const vec3 &n)
        {
            axis[2] = unit_vector(n); // w-axis

            // Choose a helper vector not parallel to n to construct u and v
            vec3 a = (std::fabs(axis[2].x()) > 0.9) ? vec3(0, 1, 0) : vec3(1, 0, 0);
            axis[1] = unit_vector(cross(axis[2], a)); // v-axis
            axis[0] = cross(axis[2], axis[1]);        // u-axis
        }

        /**
         * @brief Returns the u-axis of the basis (orthogonal to v and w).
         */
        const vec3 &u() const { return axis[0]; }

        /**
         * @brief Returns the v-axis of the basis (orthogonal to u and w).
         */
        const vec3 &v() const { return axis[1]; }

        /**
         * @brief Returns the w-axis of the basis (aligned with input normal).
         */
        const vec3 &w() const { return axis[2]; }

        /**
         * @brief Transforms a vector from local basis coordinates to world space.
         *
         * The input vector is interpreted as coordinates in the basis (u, v, w).
         * This method returns the corresponding vector in world coordinates.
         *
         * @param v The vector in basis coordinates to transform.
         * @return The transformed vector in world space.
         */
        vec3 transform(const vec3 &v) const
        {
            return (v[0] * axis[0]) + (v[1] * axis[1]) + (v[2] * axis[2]);
        }

    private:
        vec3 axis[3]; ///< The three orthonormal axes of the basis: u, v, w.
    };

}