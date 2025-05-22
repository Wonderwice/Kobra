#pragma once
#include "core/ray.h"
#include "core/hit_record.h"
#include "core/interval.h"
#include "core/aabb.h"

namespace cobra
{
    /**
     * @class hittable
     * @brief Abstract base class for all objects that can be intersected by rays.
     *
     * The hittable class defines the interface for geometric objects that can be
     * "hit" by rays in a ray tracing context. Each derived object must implement
     * the hit() function to provide ray-object intersection logic.
     */
    class hittable
    {
    public:
        /**
         * @brief Default constructor.
         */
        hittable() = default;

        /**
         * @brief Pure virtual destructor to enforce abstract class.
         */
        virtual ~hittable() = 0;

        /**
         * @brief Determines if a ray hits the object within the given range.
         *
         * This function checks if the ray intersects the object between the
         * provided t_min and t_max bounds. If a hit occurs, the hit_record
         * is populated with intersection data.
         *
         * @param r The ray to test against the object.
         * @param ray_t Interval of min and max of t.
         * @param rec The record to fill with hit information if a hit occurs.
         * @return true if the ray intersects the object, false otherwise.
         */
        virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;

        /**
         * @brief Returns the axis-aligned bounding box (AABB) of the object.
         *
         * Used for spatial acceleration structures (e.g. BVH). This must return a
         * tight-fitting bounding box that encloses the object at all times.
         *
         * @return An AABB representing the bounding volume of the object.
         */
        virtual aabb bounding_box() const = 0;
    };
    
    inline hittable::~hittable() {}
}
