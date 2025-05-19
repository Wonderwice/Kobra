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
        const vec3 _center;   ///< The center position of the sphere.
        const double _radius; ///< The radius of the sphere.

    public:
        /**
         * @brief Constructs a sphere with given center, radius, and color.
         * @param center The center of the sphere.
         * @param radius The radius of the sphere.
         * @param mat The material of the sphere.
         */
        sphere(const vec3 &center, double radius,std::shared_ptr<material> mat);

        /**
         * @brief Default destructor.
         */
        ~sphere();

        /**
         * @brief Gets the center of the sphere.
         * @return A reference to the center vector.
         */
        const vec3& center() { return _center; }

        /**
         * @brief Gets the radius of the sphere.
         * @return The radius value.
         */
        double radius() { return _radius; }

        /**
         * @brief Checks whether a ray hits the sphere between t_min and t_max.
         *
         * If the ray intersects the sphere within the specified range,
         * the hit_record is updated with intersection details such as point,
         * normal, and t value.
         *
         * @param r The ray to test.
         * @param t_min Minimum value of t for a valid intersection.
         * @param t_max Maximum value of t for a valid intersection.
         * @param rec The hit record to populate on successful intersection.
         * @return true if the ray intersects the sphere, false otherwise.
         */
        bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    };
}
