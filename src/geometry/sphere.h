#pragma once
#include "geometry/hittable.h"
#include "core/hit_record.h"

namespace cobra
{
    /**
     * @class sphere
     * @brief Represents a sphere in 3D space that can be intersected by rays.
     *
     * This class extends the hittable interface and provides ray-sphere
     * intersection logic. It stores the center, radius, and color of the sphere.
     */
    class sphere : public hittable
    {
    private:
        std::shared_ptr<material> _mat; ///< Abstraction of the object material.
        const vec3 _center;             ///< The center position of the sphere.
        const double _radius;           ///< The radius of the sphere.
        aabb bbox;                      ///< Bounding of the sphere
    public:
        /**
         * @brief Constructs a sphere with given center, radius, and color.
         * @param center The center of the sphere.
         * @param radius The radius of the sphere.
         * @param mat The material of the sphere.
         */
        sphere(const vec3 &center, double radius, std::shared_ptr<material> mat);

        /**
         * @brief Default destructor.
         */
        ~sphere();

        /**
         * @brief Gets the center of the sphere.
         * @return A reference to the center vector.
         */
        const vec3 &center() { return _center; }

        /**
         * @brief Gets the radius of the sphere.
         * @return The radius value.
         */
        double radius() { return _radius; }

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
        bool hit(const ray &r, interval ray_t, hit_record &rec) const override;

        /**
         * @brief Returns the material of the object.
         * @return A pointer to the material.
         */
        std::shared_ptr<material> mat() const { return _mat; };

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
}
