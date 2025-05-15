#pragma once
#include "math/vec3.h"

namespace cobra
{
    /**
     * @struct hit_record
     * @brief Stores information about a ray-object intersection.
     *
     * This struct holds the details of a successful ray hit, including
     * the point of intersection, the surface normal at that point, the
     * color of the object, and the distance along the ray where the hit occurred.
     */
    struct hit_record
    {
        vec3 point;   ///< The point of intersection between the ray and the object.
        vec3 normal;  ///< The normal vector at the intersection point.
        vec3 color;   ///< The color of the object at the hit point.
        double t;     ///< The ray parameter (distance from ray origin) at the intersection.
    };
} // namespace cobra
