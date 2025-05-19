#pragma once
#include "core/ray.h"
#include "core/hit_record.h"

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
    private:
        std::shared_ptr<material> _mat; //< Abstraction of the object material.

    public:
        /**
         * @brief Constructs a hittable object with a specified color.
         * @param mat The material associated with the object.
         */
        hittable(std::shared_ptr<material> mat) : _mat(mat) {};

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
         * @param t_min Minimum valid value of t for the hit.
         * @param t_max Maximum valid value of t for the hit.
         * @param rec The record to fill with hit information if a hit occurs.
         * @return true if the ray intersects the object, false otherwise.
         */
        virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;

        /**
         * @brief Returns the material of the object.
         * @return A pointer to the material.
         */
        std::shared_ptr<material> mat() const { return _mat; };
    };
    inline hittable::~hittable()
    {
    }
}
