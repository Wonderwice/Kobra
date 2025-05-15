#pragma once
#include "math/ray.h"

namespace cobra
{
    /**
     * @class camera
     * @brief A class representing a 3D camera.
     *
     * This class provides the representation of a camera in a 3D environnment.
     */
    class camera
    {
    private:
    public:

        /// Default constructor
        camera();
        /// Default destructor
        ~camera();

        /// @brief  Generate a ray from the camera to the scene.
        /// @param u row index on the grid.
        /// @param v col index on the grid.
        /// @return 
        const ray generate_ray(const double u, const double v) const;
    };
}