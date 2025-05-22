#pragma once
#include "geometry/hittable.h"
#include <vector>
#include <memory>
#include "core/aabb.h"

namespace cobra
{
    /**
     * @class scene
     * @brief Represents a 3D scene containing multiple hittable objects.
     *
     * This class manages a collection of pointers to hittable objects,
     * allowing objects to be added and accessed for ray tracing or rendering.
     */
    class scene : public hittable
    {
    public:
        std::vector<std::shared_ptr<hittable>> hittable_list; ///< List of pointers to hittable objects in the scene.
        aabb bbox;
        
        /// Default constructor
        scene();

        /// Destructor, responsible for cleaning up any allocated resources if necessary
        ~scene();

        scene(shared_ptr<hittable> obj) { add_hittable(obj); }

        /**
         * @brief Adds a hittable object to the scene.
         * @param object Pointer to the hittable object to add.
         */
        void add_hittable(std::shared_ptr<hittable> object);
        
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
        bool hit(const ray &r, interval ray_t, hit_record &rec) const override
        {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            for (const auto &object : hittable_list)
            {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
                {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }

        /**
         * @brief Returns the list of hittable objects in the scene.
         * @return A constant vector of pointers to hittable objects.
         */
        const std::vector<std::shared_ptr<hittable>> get_hittables() const;

        /**
         * @brief Returns the axis-aligned bounding box (AABB) of the object.
         *
         * Used for spatial acceleration structures (e.g. BVH). This must return a
         * tight-fitting bounding box that encloses the object at all times.
         *
         * @return An AABB representing the bounding volume of the object.
         */
        aabb bounding_box() const override { return bbox; }
    };
} // namespace cobra
