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
        
        virtual double pdf_value(const vec3 &origin, const vec3 &direction) const
        {
            return 0.0;
        }

        virtual vec3 random(const vec3 &origin) const
        {
            return vec3(1, 0, 0);
        }
    };

    inline hittable::~hittable() {}

    /**
     * @class translate
     * @brief A hittable wrapper that translates the wrapped object by a fixed offset.
     */
    class translate : public hittable
    {
    public:
        /**
         * @brief Constructor.
         * @param object The hittable object to translate.
         * @param offset The translation vector to apply.
         */
        translate(shared_ptr<hittable> object, const vec3 &offset)
            : object(object), offset(offset)
        {
            bbox = object->bounding_box() + offset;
        }

        /**
         * @brief Checks for ray intersection after offsetting the ray.
         */
        bool hit(const ray &r, interval ray_t, hit_record &rec) const override
        {
            ray offset_ray(r.get_origin() - offset, r.get_direction());

            if (!object->hit(offset_ray, ray_t, rec))
                return false;

            rec.point += offset;
            return true;
        }

        /**
         * @brief Returns the bounding box translated by the offset.
         */
        aabb bounding_box() const override
        {
            return bbox;
        }

    private:
        shared_ptr<hittable> object;
        vec3 offset;
        aabb bbox;
    };

    /**
     * @class rotate_y
     * @brief A hittable wrapper that rotates the wrapped object around the Y-axis.
     */
    class rotate_y : public hittable
    {
    public:
        /**
         * @brief Constructor.
         * @param object The hittable object to rotate.
         * @param angle Angle in degrees to rotate the object around the Y-axis.
         */
        rotate_y(shared_ptr<hittable> object, double angle) : object(object)
        {
            auto radians = degrees_to_radians(angle);
            sin_theta = std::sin(radians);
            cos_theta = std::cos(radians);
            bbox = object->bounding_box();

            vec3 min(infinity, infinity, infinity);
            vec3 max(-infinity, -infinity, -infinity);

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                        auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                        auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                        auto newx = cos_theta * x + sin_theta * z;
                        auto newz = -sin_theta * x + cos_theta * z;

                        vec3 tester(newx, y, newz);

                        for (int c = 0; c < 3; c++)
                        {
                            min[c] = std::fmin(min[c], tester[c]);
                            max[c] = std::fmax(max[c], tester[c]);
                        }
                    }
                }
            }

            bbox = aabb(min, max);
        }

        /**
         * @brief Checks for ray intersection by transforming the ray and result.
         */
        bool hit(const ray &r, interval ray_t, hit_record &rec) const override
        {
            auto origin = vec3(
                (cos_theta * r.get_origin().x()) - (sin_theta * r.get_origin().z()),
                r.get_origin().y(),
                (sin_theta * r.get_origin().x()) + (cos_theta * r.get_origin().z()));

            auto direction = vec3(
                (cos_theta * r.get_direction().x()) - (sin_theta * r.get_direction().z()),
                r.get_direction().y(),
                (sin_theta * r.get_direction().x()) + (cos_theta * r.get_direction().z()));

            ray rotated_r(origin, direction);

            if (!object->hit(rotated_r, ray_t, rec))
                return false;

            rec.point = vec3(
                (cos_theta * rec.point.x()) + (sin_theta * rec.point.z()),
                rec.point.y(),
                (-sin_theta * rec.point.x()) + (cos_theta * rec.point.z()));

            rec.normal = vec3(
                (cos_theta * rec.normal.x()) + (sin_theta * rec.normal.z()),
                rec.normal.y(),
                (-sin_theta * rec.normal.x()) + (cos_theta * rec.normal.z()));

            return true;
        }

        /**
         * @brief Returns the bounding box of the rotated object.
         */
        aabb bounding_box() const override { return bbox; }

    private:
        shared_ptr<hittable> object;
        double sin_theta;
        double cos_theta;
        aabb bbox;
    };
}