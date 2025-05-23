#pragma once
#include "core/interval.h"
#include "cobra.h"

namespace cobra
{
    /**
     * @class aabb
     * @brief Represents an Axis-Aligned Bounding Box (AABB) in 3D space.
     *
     * The AABB is defined by three intervals—one for each axis (x, y, z).
     * It is primarily used for efficient ray-object intersection testing.
     */
    class aabb
    {
    public:
        interval x, y, z; ///< Intervals along the x, y, and z axes.

        /// @brief Default constructor.
        /// Creates an empty AABB, as default-constructed intervals are empty.
        aabb() {}

        /**
         * @brief Constructs an AABB from three axis-aligned intervals.
         *
         * If any interval is too narrow (less than a small delta), it is expanded
         * slightly to avoid degenerate bounding boxes.
         *
         * @param x Interval along the x-axis.
         * @param y Interval along the y-axis.
         * @param z Interval along the z-axis.
         */
        aabb(const interval &x, const interval &y, const interval &z)
            : x(x), y(y), z(z)
        {
            double delta = 0.0001;
            if (x.size() < delta)
                this->x = x.expand(delta);
            if (y.size() < delta)
                this->y = y.expand(delta);
            if (z.size() < delta)
                this->z = z.expand(delta);
        }

        /**
         * @brief Constructs an AABB from two 3D points.
         *
         * The two points can be in any order; the constructor handles which has smaller/larger coordinates.
         *
         * @param a First corner of the box.
         * @param b Opposite corner of the box.
         */
        aabb(const vec3 &a, const vec3 &b)
        {
            x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
            y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
            z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);
        }

        /**
         * @brief Constructs a bounding box that contains two other bounding boxes.
         *
         * @param box0 The first bounding box.
         * @param box1 The second bounding box.
         */
        aabb(const aabb &box0, const aabb &box1)
        {
            x = interval(box0.x, box1.x);
            y = interval(box0.y, box1.y);
            z = interval(box0.z, box1.z);
        }

        /**
         * @brief Returns the interval on the specified axis.
         *
         * @param n Axis index: 0 = x, 1 = y, 2 = z.
         * @return Interval along the specified axis.
         */
        const interval &axis_interval(int n) const
        {
            if (n == 1)
                return y;
            if (n == 2)
                return z;
            return x;
        }

        /**
         * @brief Determines whether a ray intersects the AABB.
         *
         * Performs the slab method for ray-box intersection testing.
         *
         * @param r The ray to test.
         * @param ray_t The valid range for ray parameter t.
         * @return True if the ray hits the bounding box; false otherwise.
         */
        bool hit(const ray &r, interval ray_t) const
        {
            const vec3 &ray_orig = r.get_origin();
            const vec3 &ray_dir = r.get_direction();

            for (int axis = 0; axis < 3; axis++)
            {
                const interval &ax = axis_interval(axis);
                const double adinv = 1.0 / ray_dir[axis];

                auto t0 = (ax.min - ray_orig[axis]) * adinv;
                auto t1 = (ax.max - ray_orig[axis]) * adinv;

                if (t0 < t1)
                {
                    if (t0 > ray_t.min)
                        ray_t.min = t0;
                    if (t1 < ray_t.max)
                        ray_t.max = t1;
                }
                else
                {
                    if (t1 > ray_t.min)
                        ray_t.min = t1;
                    if (t0 < ray_t.max)
                        ray_t.max = t0;
                }

                if (ray_t.max <= ray_t.min)
                    return false;
            }
            return true;
        }
    };

    inline aabb operator+(const aabb &bbox, const vec3 &offset)
    {
        return aabb(bbox.x + offset.x(), bbox.y + offset.y(), bbox.z + offset.z());
    }

    inline aabb operator+(const vec3 &offset, const aabb &bbox)
    {
        return bbox + offset;
    }
}
