#include "camera.h"

namespace cobra
{
    // ----------------------------
    // Constructors & Destructors
    // ---------------------------

    camera::camera()
    {
    }

    camera::~camera()
    {
    }

    // ----------------------------
    // Ray generation
    // ---------------------------

    const ray camera::generate_ray(const double u, const double v) const
    {
        return ray(vec3(0, 0, 0), vec3(0, 0, 0));
    }
}