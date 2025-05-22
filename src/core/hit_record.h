#pragma once
#include "core/vec3.h"
#include <memory>

namespace cobra
{
    class material;

    /**
     * @class hit_record
     * @brief Stores information about a ray-object intersection.
     *
     * This struct holds the details of a successful ray hit, including
     * the point of intersection, the surface normal at that point, the
     * color of the object, and the distance along the ray where the hit occurred.
     */
    class hit_record
    {
    public:
        vec3 point;                    ///< The point of intersection between the ray and the object.
        vec3 normal;                   ///< The normal vector at the intersection point.
        double t;                      ///< The ray parameter (distance from ray origin) at the intersection.
        bool front_face;               ///< Front-face tracking
        std::shared_ptr<material> mat; ///< Material representation
        double u;                      ///< Texture coordinate (latitude)
        double v;                      ///< Texture coordinate (longitude)

        /**
         * @brief Sets the hit record normal vector.
         * @param ray The ray which intersect.
         * @param outward_normal The normal at intersection point.
         */
        void set_face_normal(const ray &r, const vec3 &outward_normal)
        {
            front_face = dot(r.get_direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };
} // namespace cobra
