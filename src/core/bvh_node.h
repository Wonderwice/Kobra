#pragma once
#include "core/aabb.h"
#include "geometry/hittable.h"
#include "scene/scene.h"
#include "cobra.h"

#include <algorithm>

namespace cobra
{
    /**
     * @class bvh_node
     * @brief A node in a Bounding Volume Hierarchy (BVH) for fast ray-object intersection.
     *
     * BVH is a binary tree structure that partitions the scene's hittable objects for efficient traversal.
     * Each node stores a bounding box containing its child nodes or geometry.
     */
    class bvh_node : public hittable
    {
    public:
        /**
         * @brief Constructs a BVH tree from a scene.
         *
         * Copies the hittable list from the scene and builds a hierarchical structure from it.
         * 
         * @param world The scene containing hittable objects.
         */
        bvh_node(scene world)
            : bvh_node(world.hittable_list, 0, world.hittable_list.size())
        {
            // C++ subtlety: this constructor copies hittable_list temporarily;
            // it's okay since the BVH structure gets built during the call.
        }

        /**
         * @brief Recursively constructs a BVH from a subset of hittables.
         *
         * Selects a random axis (x, y, z), sorts the objects along that axis, and divides them.
         *
         * @param objects List of shared pointers to hittables.
         * @param start Start index (inclusive).
         * @param end End index (exclusive).
         */
        bvh_node(std::vector<shared_ptr<hittable>> &objects, size_t start, size_t end)
        {
            int axis = random_int(0, 2);
            auto comparator = (axis == 0) ? box_x_compare
                              : (axis == 1) ? box_y_compare
                                            : box_z_compare;

            size_t object_span = end - start;

            if (object_span == 1)
            {
                left = right = objects[start];
            }
            else if (object_span == 2)
            {
                left = objects[start];
                right = objects[start + 1];
            }
            else
            {
                std::sort(objects.begin() + start, objects.begin() + end, comparator);
                auto mid = start + object_span / 2;
                left = make_shared<bvh_node>(objects, start, mid);
                right = make_shared<bvh_node>(objects, mid, end);
            }

            bbox = aabb(left->bounding_box(), right->bounding_box());
        }

        /**
         * @brief Tests if a ray hits any object in the BVH.
         *
         * First tests the bounding box. If successful, recursively tests child nodes.
         *
         * @param r The incoming ray.
         * @param ray_t The valid interval for ray parameter t.
         * @param rec The hit record to store intersection info.
         * @return True if the ray hits any object in the subtree.
         */
        bool hit(const ray &r, interval ray_t, hit_record &rec) const override
        {
            if (!bbox.hit(r, ray_t))
                return false;

            bool hit_left = left->hit(r, ray_t, rec);
            bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

            return hit_left || hit_right;
        }

        /**
         * @brief Returns the AABB bounding the entire subtree rooted at this node.
         * @return The bounding box.
         */
        aabb bounding_box() const override { return bbox; }

    private:
        shared_ptr<hittable> left; ///< Left child node or object.
        shared_ptr<hittable> right; ///< Right child node or object.
        aabb bbox; ///< Bounding box for this node.

        /**
         * @brief Compares two hittables based on their bounding box's minimum coordinate along an axis.
         * @param a First hittable.
         * @param b Second hittable.
         * @param axis_index Axis (0 = x, 1 = y, 2 = z).
         * @return True if a < b on the given axis.
         */
        static bool box_compare(
            const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index)
        {
            auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
            auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
            return a_axis_interval.min < b_axis_interval.min;
        }

        /// Comparator for x-axis.
        static bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
        {
            return box_compare(a, b, 0);
        }

        /// Comparator for y-axis.
        static bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
        {
            return box_compare(a, b, 1);
        }

        /// Comparator for z-axis.
        static bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
        {
            return box_compare(a, b, 2);
        }
    };
} // namespace cobra
