#pragma once
#include "geometry/hittable.h"
#include <vector>

namespace cobra
{
    /**
     * @class scene
     * @brief Represents a 3D scene containing multiple hittable objects.
     *
     * This class manages a collection of pointers to hittable objects,
     * allowing objects to be added and accessed for ray tracing or rendering.
     */
    class scene
    {
    private:
        std::vector<hittable*> hittable_list; ///< List of pointers to hittable objects in the scene.

    public:
        /// Default constructor
        scene();

        /// Destructor, responsible for cleaning up any allocated resources if necessary
        ~scene();

        /**
         * @brief Adds a hittable object to the scene.
         * @param object Pointer to the hittable object to add.
         */
        void add_hittable(hittable* object);

        /**
         * @brief Returns the list of hittable objects in the scene.
         * @return A constant vector of pointers to hittable objects.
         */
        const std::vector<hittable*> get_hittables() const;
    };
} // namespace cobra
